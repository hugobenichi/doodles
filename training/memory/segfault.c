#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void dumpstack(void){
    /* Got this routine from http://www.whitefang.com/unix/faq_toc.html
    ** Section 6.5. Modified to redirect to file to prevent clutter
    */
    /* This needs to be changed... */
    char dbx[160];

    sprintf(dbx, "gcore %d", getpid());
    /* Change the dbx to gdb */

//    system(dbx); /* does not work */
    return;
}

static void handler(int signal){

    if ( signal == SIGSEGV || signal == SIGBUS) {
        printf("seg fault\n");
        dumpstack();
    }
    exit(-1);
}

int main(void) {

    struct sigaction action;

    action.sa_handler = handler;
    action.sa_flags = 0;
    
    sigemptyset(&action.sa_mask);

    sigaction(SIGSEGV, &action, NULL);
    sigaction(SIGBUS, &action, NULL);

    int somewhere = 0;
    int* hell = &somewhere;

    int i = 0;
    while(1) *(hell + i++) = 666;

}
