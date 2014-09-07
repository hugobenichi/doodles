

#include	"frames/frames.h"


EXT frames* frames_alloc
  (int shot_num, int tomo_num, int siz)
{
  int i;

  frames* frms = (frames*) malloc(sizeof(frames));

  printf(
    "alloc new frames\nshot_num: %i  tomo_num: %i  frm_siz: %i\n"
    , shot_num, tomo_num, siz
  );

  frms->shot_n = shot_num;
  frms->tomo_n = tomo_num;
  frms->frm_siz = siz;

  frms->shots   = (char**)  malloc(sizeof(char*)*shot_num);
  frms->frames  = (char**)  malloc(sizeof(char*)*tomo_num);
  frms->phases  = (double*) malloc(sizeof(double)*tomo_num);

  for (i = 0; i < shot_num; i++)
    frms->shots[i]  = (char*) malloc(sizeof(char)*siz);

  for (i = 0; i < tomo_num; i++)
    frms->frames[i] = (char*) malloc(sizeof(char)*siz);

  return frms;

}


EXT void frames_free
  (frames* frms)
{

  int i;
  for (i = 0; i < frms->shot_n; i++)
    free(frms->shots[i]);
  for (i = 0; i < frms->tomo_n; i++)
    free(frms->frames[i]);

  free(frms->shots);
  free(frms->frames);
  free(frms->phases);
  free(frms);

}


EXT void frames_copy_shot
  (frames* frms, char* buffer, int buf_siz, int* i_frm, int* i_tot)
{
  int i = 0;
  //int *i_frm = &(frms->index_frm);
  //int *i_tot = &(frms->index_tot);
  
  //printf("copy, s_buf: %i  i_tot: %i  i_frm: %i", buf_siz, *i_tot, *i_frm);
  
  printf("copy shot, i_tot: %i  i_frm: %i", *i_tot, *i_frm);
  while(buf_siz) {

    while (frms->frm_siz - *i_frm && buf_siz) {
      
      frms->shots[*i_tot][*i_frm] = buffer[i];
      (*i_frm)++;
      buf_siz--;
      i++;

    }

    if (frms->frm_siz == *i_frm) {
      *i_frm = 0;
      (*i_tot)++;
    }

  }
  //printf("  i_buf: %i  i_tot: %i  i_frm %i\n", i, *i_tot, *i_frm);
}


EXT void frames_copy_tomo
  (frames* frms, char* buffer, int buf_siz, int* i_frm, int* i_tot)
{
  int i = 0;
  //int *i_frm = &(frms->index_frm);
  //int *i_tot = &(frms->index_tot);

  printf("copy tomo, i_tot: %i  i_frm: %i", *i_tot, *i_frm);
  while(buf_siz) {

    while (frms->frm_siz - *i_frm && buf_siz) {
      
      frms->frames[*i_tot][*i_frm] = buffer[i];
      (*i_frm)++;
      buf_siz--;
      i++;

    }

    if (frms->frm_siz == *i_frm) {
      *i_frm = 0;
      (*i_tot)++;
    }

  }

}


EXT void frames_copy
  (char** data, int frm_siz, char* buffer, int buf_siz, int* i_frm, int* i_tot)
{
  int i = 0;
  
  //printf("copy, i_tot: %i  i_frm: %i", *i_tot, *i_frm);
  while(buf_siz) {

    while (frm_siz - *i_frm && buf_siz) {
      
      data[*i_tot][*i_frm] = buffer[i];
      (*i_frm)++;
      buf_siz--;
      i++;

    }

    if (frm_siz == *i_frm) {
      *i_frm = 0;
      (*i_tot)++;
    }

  }

}


EXT void frames_copy_to_shot
  (frames* frm, char* buffer, int index)
{
  int i = 0;

  while (frm->frm_siz - i) {
    frm->shots[index][i] = buffer[i];
    i++;
  }

}


EXT void frames_copy_to_tomo
  (frames* frm, char* buffer, int index)
{
  int i = 0;

  while (frm->frm_siz - i) {
    frm->frames[index][i] = buffer[i];
    i++;
  }

}


EXT void frames_apply_window 
  (frames* frms, double* window, double* shot_out, double* quad_out)
{
  int i,j;

  i = 0;
  while(frms->shot_n - i) {
    shot_out[i] = 0;

    j = 0;
    while (frms->frm_siz - j) {
      shot_out[i] += (frms->shots[i][j])*(window[j]); 
      j++;
    }

    i++;
  }

  i = 0;
  while(frms->tomo_n - i) {
    quad_out[i] = 0;

    j = 0;
    while (frms->frm_siz - j) {
      quad_out[i] += (frms->frames[i][j])*(window[j]); 
      j++;
    }

    i++;
  }
 
}


EXT void frames_apply_window_smth 
  (frames* frms, double* window, double* shot_out, double* quad_out, int avr)
{

  double* buffer = (double*) malloc( sizeof(double)*(frms-> frm_siz) );
  int i,j;

  i = 0;
  while(frms->shot_n - i) {
    frames_smooth( frms-> shots[i], frms-> frm_siz, avr, buffer );
    shot_out[i] = 0;

    j = 0;
    while (frms->frm_siz - j) {
      shot_out[i] += (buffer[j])*(window[j]); 
      j++;
    }

    i++;
  }

  i = 0;
  while(frms->tomo_n - i) {
    frames_smooth( frms-> frames[i], frms-> frm_siz, avr, buffer );
    quad_out[i] = 0;

    j = 0;
    while (frms->frm_siz - j) {
      quad_out[i] += (buffer[j])*(window[j]); 
      j++;
    }

    i++;
  }
  
  free(buffer);
 
}


EXT double* 
frames_mean               
(frames* f)
{

  int i, j, k = 0,
      sht_n = f-> shot_n, 
      tom_n = f-> tomo_n,
      frm_s = f-> frm_siz;

  double* means = (double*) malloc( sizeof(double) * (sht_n + tom_n) );

  for (i = 0; i < sht_n; i++, k++) {
    means[k] = 0.0;
    for (j = 0; j < frm_s; j++)
      means[k] += 1.0 * f-> shots[i][j];
    means[k] /= frm_s;
  }

  for (i = 0; i < tom_n; i++, k++) {
    means[k] = 0.0;
    for (j = 0; j < frm_s; j++)
      means[k] += 1.0 * f-> frames[i][j];
    means[k] /= frm_s;
  }

  return means;

}


EXT void
frames_remove_dc          
(frames* f)
{

  int i, j,
      sht_n = f-> shot_n, 
      tom_n = f-> tomo_n,
      frm_s = f-> frm_siz;

  double mean;
  char *frm;

  for (i = 0; i < sht_n; i++) 
  {
    mean = 0.0;
    frm  = f->shots[i];
    for (j = 0; j < frm_s; j++)
      mean += 1.0 * frm[j];
    mean /= frm_s;
    for (j = 0; j < frm_s; j++)
      frm[j] -= (char) mean;
  }

  for (i = 0; i < tom_n; i++) 
  {
    mean = 0.0;
    frm  = f->frames[i];
    for (j = 0; j < frm_s; j++)
      mean += 1.0 * frm[j];
    mean /= frm_s;
    for (j = 0; j < frm_s; j++)
     frm[j] -= (char) mean;
  }

}


EXT void
frames_remove_meandc          
(frames* f)
{

  int i, j,
      sht_n = f-> shot_n, 
      tom_n = f-> tomo_n,
      frm_s = f-> frm_siz;

  double mean, *frm;

  mean = 0.0;
  for (i = 0; i < sht_n; i++) 
    for (j = 0; j < frm_s; j++)
      mean += 1.0 * f->shots[i][j];

  mean /= (sht_n * frm_s);
  for (i = 0; i < sht_n; i++) 
    for (j = 0; j < frm_s; j++)
      f->shots[i][j] -= (char) mean;

  mean = 0.0;
  for (i = 0; i < tom_n; i++) 
    for (j = 0; j < frm_s; j++)
      mean += 1.0 * f->frames[i][j];

  mean /= (tom_n * frm_s);
  for (i = 0; i < tom_n; i++) 
    for (j = 0; j < frm_s; j++)
      f->frames[i][j] -= (char) mean;

}


EXT void var_addto
  (char* data, double* var, int siz)
{
  int i = 0;

  while(siz-i) {
    var[i] += (double) (data[i])*(data[i]);
    i++;
  }

}


EXT void var_addto_smth
  (char* data, double* var, int siz, int avr)
{
  int i = 0;

  double* buffer = (double*) malloc( sizeof(double)*siz );  
  frames_smooth( data, siz, avr, buffer );

  while(siz-i) {
    var[i] += (buffer[i])*(buffer[i]);
    i++;
  }

  free(buffer);

}


EXT double mode_apply
  (char* data, double* mode, int siz)
{
  int i = 0;
  double x = 0.0;

  while(siz-i) {
    x += (double) (data[i])*(mode[i]);
    i++;
  }

  return x/siz;
}


EXT double var_comp
  (double* data, int num)
{
  int i;
  double mean = 0.0, var = 0.0;

  i = 0;
  while(num-i) {
    mean += data[i];
    var  += data[i]*data[i];
    i++;
  }

  mean /= num;
  var  /= num; 

  double sigma = sqrt(var-mean*mean);
  printf("shotnoise mean: %f\nshotnoise sigma:  %f\n",mean, sigma);

  return sigma;

}


EXT void norm
  (double* data, double norm, int num)
{
  int i = 0;
  double x = (1.0)/norm;

  while(num-i) {
    data[i] *= x;
    i++;
  }

}


EXT void phase_acos  
  (double* data, int num)
{
  int i = 0;
  
  while(num-i) {
    data[i] = acos(data[i]);
    i++;
  }

}


EXT void frames_smooth
  (char* in_data, int siz, int avr, double* out_data)
{


  double cur = 0.0;
  int n = 0;
  int i;  

  for (i = 0; i < siz; i++) {
    cur += (double) in_data[i];
    if (i < avr)
      n += 1;
    else
      cur -= (double) in_data[i-avr]; 
    out_data[i] = cur / n;
  }

/*
  double cur = (avr-1)*in_data[0];
  int i;  

  for (i = 0; i < siz; i++) {
    cur += (double) in_data[i];
    cur -= (double) ( i < avr ? in_data[0] : in_data[i-avr]);
    out_data[i] = cur / avr;
  }
*/
}


