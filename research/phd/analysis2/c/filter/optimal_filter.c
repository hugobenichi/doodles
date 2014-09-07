#include	"optimal_filter.h"


EXT() frames_double* frames_double_alloc
  (frames_data* old)
{
  frames_double* new = (frames_double*) malloc(sizeof(frames_double));

  int i, j;

  new-> shot_n  = old-> shot_n;
  new-> tomo_n  = old-> tomo_n;
  new-> frm_siz = old-> frm_siz;

  new-> shots   = (double**) malloc(sizeof(double*)*(new-> shot_n));
  new-> frames  = (double**) malloc(sizeof(double*)*(new-> tomo_n));
  new-> phases  = (double*)  malloc(sizeof(double)*(new-> tomo_n));

  for (i = 0; i < new-> shot_n; i++) {
    new-> shots[i]  = (double*) malloc(sizeof(double)*(new-> frm_siz));

    for (j = 0; j < new-> frm_siz; j++)
      new-> shots[i][j] = (double) old-> shots[i][j];

  }

  for (i = 0; i < new-> tomo_n; i++) {
    new-> frames[i] = (double*) malloc(sizeof(double)*(new-> frm_siz));

    for (j = 0; j < new-> frm_siz; j++)
      new-> frames[i][j] = (double) old-> frames[i][j];

  }

  for (j = 0; j < new-> tomo_n; j++)
    new-> phases[j] = old-> phases[j];

  return new;  

}


EXT() void frames_double_free
  (frames_double* frms)
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

EXT() void frames_fft_transform
  (frames_double* frms)
{

  frms-> realwave  = gsl_fft_real_wavetable_alloc(frms-> frm_siz);
  frms-> workspace = gsl_fft_real_workspace_alloc(frms-> frm_siz);

  int i;

  for (i = 0; i < frms-> shot_n; i++)
    gsl_fft_real_transform( 
      frms-> shots[i], 
      1, 
      frms-> frm_siz, 
      frms-> realwave,
      frms-> workspace      
    );

  for (i = 0; i < frms-> tomo_n; i++)
    gsl_fft_real_transform( 
      frms-> frames[i], 
      1, 
      frms-> frm_siz, 
      frms-> realwave,
      frms-> workspace      
    );

  gsl_fft_real_wavetable_free(frms-> realwave);

}


EXT() void frames_fft_backward
  (frames_double* frms)
{

  frms-> halfcomplexwave = gsl_fft_halfcomplex_wavetable_alloc(frms-> frm_siz);

  int i;

  for (i = 0; i < frms-> shot_n; i++)
    gsl_fft_halfcomplex_transform( 
      frms-> shots[i], 
      1, 
      frms-> frm_siz, 
      frms-> halfcomplexwave,
      frms-> workspace      
    );

  for (i = 0; i < frms-> tomo_n; i++)
    gsl_fft_halfcomplex_transform( 
      frms-> frames[i], 
      1, 
      frms-> frm_siz, 
      frms-> halfcomplexwave,
      frms-> workspace      
    );

  gsl_fft_halfcomplex_wavetable_free(frms-> halfcomplexwave);
  gsl_fft_real_workspace_free(frms-> workspace);

}


EXT() void frames_apply_filter
  (frames_double* frms, Real* filter)
{

  int i, j, k;

  for (i = 0; i < frms-> shot_n; i++) {

    for (j = 0; j < frms-> frm_siz - 1; j++) 
      frms-> shots[i][j] *= (double) filter[(j+1)/2];

    frms-> shots[i][j] *= (double) filter[j/2];

  }

  for (i = 0; i < frms-> tomo_n; i++) {

    for (j = 0; j < frms-> frm_siz - 1; j++) 
      frms-> frames[i][j] *= (double) filter[(j+1)/2];

    frms-> frames[i][j] *= (double) filter[j/2];

  }

}


EXT() void frames_filter
  (frames_double* frms, Real* filter)
{

  frames_fft_transform(frms);

  frames_apply_filter(frms, filter);
  
  frames_fft_backward(frms);

}

EXT() void frames_square_filter
  (frames_double* frms, int n)
{

  int i, j;
  
  for (i = 0; i < frms-> shot_n; i++)
    for (j = n; j < frms-> frm_siz; j++)
      frms-> shots[i][j] = 0.0;

  for (i = 0; i < frms-> tomo_n; i++)
    for (j = n; j < frms-> frm_siz; j++)
      frms-> frames[i][j] = 0.0;

}

EXT() void frames_trapezoid_filter
  (frames_double* frms, int a, int b)
{

  double alpha = (double) 1.0/(a-b); 
  double beta  = (double) (b*1.0)/(b-a); 
  int i, j;
  
  for (i = 0; i < frms-> shot_n; i++)
    for (j = b; j < frms-> frm_siz; j++)
      frms-> shots[i][j] = 0.0;

  for (i = 0; i < frms-> shot_n; i++)
    for (j = a; j < b; j++)
      frms-> shots[i][j] = alpha*2*((j+1)/2)+beta;

  for (i = 0; i < frms-> tomo_n; i++)
    for (j = b; j < frms-> frm_siz; j++)
      frms-> frames[i][j] = 0.0;

  for (i = 0; i < frms-> tomo_n; i++)
    for (j = a; j < b; j++)
      frms-> frames[i][j] = alpha*2*((j+1)/2)+beta;

}

EXT() void frames_nodc_filter
  (frames_double* frms)
{

  int i;
  
  for (i = 0; i < frms-> shot_n; i++)
    frms-> shots[i][0] = 0.0;

  for (i = 0; i < frms-> tomo_n; i++)
    frms-> frames[i][0] = 0.0;

}


EXT() void frames_double_smooth
  (double* in_data, int siz, int avr, double* out_data)
{

  double cur = 0.0;
  int n = 0;
  int i;  

  for (i = 0; i < siz; i++) {
    cur += in_data[i];
    if (i < avr)
      n += 1;
    else
      cur -= in_data[i-avr]; 
    out_data[i] = cur / n;
  }

}


EXT() void frames_var_addto
  (double* data, Real* var, int siz)
{
  int i = 0;

  while(siz-i) {
    var[i] += (data[i])*(data[i]);
    i++;
  }

}


EXT() void frames_var_addto_smth
  (double* data, Real* var, int siz, int avr)
{
  int i = 0;

  double* buffer = (double*) malloc( sizeof(double)*siz );  
  frames_double_smooth( data, siz, avr, buffer );

  while(siz-i) {
    var[i] += (buffer[i])*(buffer[i]);
    i++;
  }

  free(buffer);

}


EXT() void frames_window 
  (frames_double* frms, double* window, double* shot_out, double* quad_out)
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


EXT() void frames_window_smth 
  (frames_double* frms, double* window, double* shot_out, double* quad_out, int avr)
{

  double* buffer = (double*) malloc( sizeof(double)*(frms-> frm_siz) );
  int i,j;

  i = 0;
  while(frms->shot_n - i) {
    frames_double_smooth( frms-> shots[i], frms-> frm_siz, avr, buffer );
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
    frames_double_smooth( frms-> frames[i], frms-> frm_siz, avr, buffer );
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

