

#ifndef QUIX_MEMORY
  #define QUIX_MEMORY


  #include    <stdlib.h>
  #include    <stdio.h>
  #include    "quix/extern.h"
  #include    "quix/predicat.h"

  typedef     void    (*quix_object_allocator)        (int);
  typedef     int     (*quix_object_desallocator)     (int);

  EXT         void    quix_memory_set_allocator       (quix_object_allocator);
  EXT         void    quix_memory_set_desallocator    (quix_object_desallocator);

              void*   quix_object_alloc_function      (int);
              int     quix_object_free_function       (void*);


  #define     quix_object_alloc(type)             (type*) quix_object_alloc_function(sizeof(type))
  #define     quix_object_free(pointer)           quix_object_free_function((void*)pointer)


  #define     quix_alloc(object,n)     (object*) malloc( sizeof(object) * (n) )
  #define     double_alloc(n)           quix_alloc(double,n)

  #define     alloc_check( pointer, MSG )\
                if( unlikely(pointer == NULL) )\
                {\
                  printf("QUIX NULL alloc error: " MSG "\n");\
                  return NULL;\
                }\

  #define     free_if(pointer)     if (pointer != NULL ) free(pointer);


  #define     already_free(MSG)         printf( MSG " pointer already freed\n");
  #define     already_null(MSG)         printf( MSG " pointer already NULL\n");


#endif


