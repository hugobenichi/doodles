#ifndef FRAMES
  #define FRAMES


  #include  <stdlib.h>
  #include	<stdio.h>
  #include  <math.h>


  #define     PI          3.1411592654


  #ifdef __WIN32__
    #define EXT extern __declspec(dllexport)
  #else
    #define EXT
  #endif


  typedef 
    struct frames_proto {
      int       shot_n;
      int       tomo_n;
      int       frm_siz;
      char**    shots;
      char**    frames;
      double*   phases;
    } 
    frames;


  EXT   frames*         frames_alloc              (int, int, int);
  EXT   void            frames_free               (frames*);

  EXT   void            frames_copy_shot          (frames*, char*, int, int*, int*);
  EXT   void            frames_copy_tomo          (frames*, char*, int, int*, int*);
  EXT   void            frames_copy               (char**, int, char*, int, int*, int*);
  EXT   void            frames_copy_to_shot       (frames*, char*, int);
  EXT   void            frames_copy_to_tomo       (frames*, char*, int);

  EXT   void            frames_apply_window       (frames*, double*, double*, double*);
  EXT   void            frames_apply_window_smth  (frames*, double*, double*, double*, int);

  EXT   double*         frames_mean               (frames*);
  EXT   void            frames_remove_dc          (frames*);
  EXT   void            frames_remove_meandc      (frames*);


  EXT   void            var_addto                 (char*, double*, int);
  EXT   void            var_addto_smth            (char*, double*, int, int);
  EXT   double          mode_apply                (char*, double*, int);
  EXT   double          var_comp                  (double*, int);
  EXT   void            norm                      (double*, double, int);
  EXT   void            phase_acos                (double*, int);

  EXT   void            frames_smooth             (char*, int, int, double*);


#endif


