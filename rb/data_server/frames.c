#include	"frames.h"

EXT() frames_data* frames_alloc
  (int shot_num, int tomo_num, int siz, int byt)
{
  int i;

  frames_data* frms = (frames_data*) malloc(sizeof(frames_data));

  printf(
    "alloc new frames\nshot_num: %i  tomo_num: %i  frm_siz: %i\n"
    , shot_num, tomo_num, siz
  );

  frms->shot_n = shot_num;
  frms->tomo_n = tomo_num;
  frms->frm_siz = siz;
  frms->byt_siz = byt;

  frms->shots   = (char**)  malloc(sizeof(char*)*shot_num);
  frms->frames  = (char**)  malloc(sizeof(char*)*tomo_num);
  frms->phases  = (double*) malloc(sizeof(double)*tomo_num);

  for (i = 0; i < shot_num; i++)
    frms->shots[i]  = (char*) malloc(sizeof(char)*siz*byt);

  for (i = 0; i < tomo_num; i++)
    frms->frames[i] = (char*) malloc(sizeof(char)*siz*byt);

  return frms;

}

EXT() void frames_free
  (frames_data* frms)
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

EXT() void frames_copy_shot
  (frames_data* frms, char* buffer, int buf_siz, int* i_frm, int* i_tot)
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

EXT() void frames_copy_tomo
  (frames_data* frms, char* buffer, int buf_siz, int* i_frm, int* i_tot)
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

EXT() void frames_copy
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

EXT() void frames_apply_window 
  (char** data, int frm_num, int frm_siz, double* window, double* data_out)
{
  int i = 0,j;

  while(frm_num-i) {
    data_out[i] = 0;

    j = 0;
    while (frm_siz-j) {
      data_out[i] += (data[i][j])*(window[j]); 
      j++;
    }

    i++;
  }
 
}

