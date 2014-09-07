#ifndef _FRAME_DATA_
#define _FRAME_DATA_

#include  <stdlib.h>
#include	<stdio.h>
//#include  <memory.h>

#ifdef __WIN32__
  #define EXT(x) extern __declspec(dllexport) x
#else
  #define EXT(x) x
#endif

#define 	DLL 			extern __declspec(dllexport)

typedef struct frames_data_proto {
  int       shot_n;
  int       tomo_n;
  int       frm_siz;
  int       byt_siz;
  char**    shots;
  char**    frames;
  double*   phases;
} frames_data;

EXT() frames_data* frames_alloc(int shot_num, int tomo_num, int siz, int byt);
EXT() void frames_free(frames_data* frms);
EXT() void frames_copy_shot(frames_data* frm, char* buffer, int buf_siz, int* i_frm, int* i_tot);
EXT() void frames_copy_tomo(frames_data* frm, char* buffer, int buf_siz, int* i_frm, int* i_tot);
EXT() void frames_copy(char** data, int frm_siz, char* buffer, int buf_siz, int* i_frm, int* i_tot);
EXT() void frames_apply_window(char** data, int frm_num, int frm_siz, double* window, double* data_out);

  
#endif

