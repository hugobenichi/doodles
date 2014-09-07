

#include "quix/parallel.h"
#include <stdio.h>


#if PARALLEL>0

  static    int               jobs_active = 0;
  static    pthread_t         jobs[N_Thread];
  static    pthread_attr_t    jobs_attribute[N_Thread];

#endif


int 
job_start
(executable job, void* args)
{

  #if PARALLEL>0   /* delegate to a thread (serial exec) */

    if (jobs_active == N_Thread) return -1;

    pthread_t*      thread = &jobs[jobs_active];
    pthread_attr_t* attrib = &jobs_attribute[jobs_active];

    pthread_attr_init( attrib );
    pthread_attr_setdetachstate( attrib, PTHREAD_CREATE_JOINABLE );
    pthread_create(thread, attrib, job, args);

    jobs_active++;

    return jobs_active;

  #else     /* execute immediately the task to do (serial exec) */

    job(args);

    return 0;

  #endif

}


void              
job_wait                     
(int job_num)
{

  #if PARALLEL>0
    void*             dummy;
    pthread_t*        thread  = &jobs[job_num-1];
    pthread_attr_t*   attrib  = &jobs_attribute[job_num-1];

    pthread_join(*thread, &dummy);
    pthread_attr_destroy(attrib);

    jobs_active--;
  #endif

}


void
job_wait_all         
(void)
{

  #if PARALLEL>0
    if (jobs_active) {
      do { job_wait(jobs_active+1); } while(jobs_active);
    }
  #endif

}


void
job_finish                  
()
{
                                
  #if PARALLEL>0                  
    pthread_exit(NULL);           
  #endif

}


