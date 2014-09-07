

#include "quix/memory.h"


static  void  quix_default_allocator(int);
static  int   quix_default_desallocator(int);


static  quix_object_allocator       allocator    = quix_default_allocator;
static  quix_object_desallocator    desallocator = quix_default_desallocator;


void*       
quix_object_alloc_function          
(int size)
{

  void* pointer = malloc( size );
  alloc_check(pointer, "quix_object_alloc");
  allocator((int)pointer);
  return pointer;

}


int         
quix_object_free_function           
(void* pointer)
{

  return desallocator((int)pointer);

}


EXT void
quix_memory_set_allocator           
(quix_object_allocator new_allocator)
{
  allocator = new_allocator;
}


EXT void
quix_memory_set_desallocator        
(quix_object_desallocator new_desallocator)
{
  desallocator = new_desallocator;  
}


static void  
quix_default_allocator(int address)
{}


static int   
quix_default_desallocator(int address)
{
  return 1;
}


