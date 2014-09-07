#include <stdio.h>


#if defined(_WIN32)
  #define 	DLL     extern __declspec(dllexport)
#else
  #define   DLL
#endif


typedef   struct cbuffer    cbuffer;
typedef   struct link       link;

typedef   int (*reader)(char*,void*);

struct
  cbuffer               // 192 bytes on 32bits machines and 352 bytes on 64bits machines
  {
    link*   inbound;    // next buffer memory for incoming data
    link*   outbound;   // next buffer memory for writing
    link*   start;      // start of the memory (used for cleaning and init)
    link*   stop;       // where to write to
    FILE*   output;     // file
    int     chunk;      // size of one chunk
  };


struct
  link
  {
    char* mem;
    link* next;
  };


DLL   cbuffer*  cbuffer_new       (int chunk, int size);      //  size is n_chunk * chunk
DLL   void      cbuffer_free      (cbuffer*);                 // don't forget to close output if open
DLL   int       cbuffer_open      (cbuffer*, const char* );   // open a new file at the given path

DLL   char*     cbuffer_next      (cbuffer*);                 // blocking, return the next buffer memory inline
DLL   char*     cbuffer_has_next  (cbuffer*);                 // non-blocking, return null if not ready
DLL   void      cbuffer_stop      (cbuffer*);                 // flag for stopping (cut the circular list)
DLL   int       cbuffer_write     (cbuffer*);                 // blocking, write everything untill stop
DLL   int       cbuffer_read      (cbuffer*, reader, void*);  // blocking, passes memory chunk written to to a user defined handling function 'reader'

