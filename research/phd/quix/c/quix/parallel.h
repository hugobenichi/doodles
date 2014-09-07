

#ifndef QUIX_PARALLEL_H
  #define QUIX_PARALLEL_H

  #ifdef __WIN32__

    #define PARALLEL    0
    #define N_Thread    1

  #else

    #define PARALLEL    1
    #define N_Thread    32

    #include <sched.h>  //not necessary maybe
    #include <pthread.h> 

  #endif

  typedef   void* (*executable)(void*);

  int                     job_start                       (executable, void*);
  void                    job_wait                        (int);
  void                    job_wait_all                    (void);
  void                    job_finish                      (void);

#endif


