#ifndef _OPT_FILTER_
  #define _OPT_FILTER_

  #include "frames.h"         //attention recupere tt les declarations de ce header
  #include "gsl/gsl_errno.h"
  #include "gsl/gsl_fft_real.h"
  #include "gsl/gsl_fft_halfcomplex.h"

typedef struct frames_double_proto {
  int       shot_n;
  int       tomo_n;
  int       frm_siz;
  double**  shots;
  double**  frames;
  double*   phases;
  gsl_fft_real_workspace* workspace;
  gsl_fft_real_wavetable* realwave;
  gsl_fft_halfcomplex_wavetable* halfcomplexwave;
} frames_double;

EXT() frames_double*  frames_double_alloc(frames_data*);
EXT() void            frames_double_free(frames_double*);
     
EXT() void            frames_fft_transform(frames_double*);
EXT() void            frames_fft_backward(frames_double*);

EXT() void            frames_apply_filter(frames_double*, Real* filter);
EXT() void            frames_filter(frames_double*, Real* filter);
EXT() void            frames_square_filter(frames_double*, int);
EXT() void            frames_trapezoid_filter(frames_double*, int, int);
EXT() void            frames_nodc_filter(frames_double*);

EXT() void            frames_double_smooth (double*, int, int, double*);

EXT() void            frames_var_addto(double* data, Real* var, int siz);
EXT() void            frames_var_addto_smth(double*, Real*, int, int avr);
EXT() void            frames_window(frames_double*, double* win, double* sht_out, double* qd_out);
EXT() void            frames_window_smth(frames_double*, double*n, double*, double*,int avr);

#endif

