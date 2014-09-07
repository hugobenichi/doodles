/*
 *  
 *                          CBuffer 
 *    
 *  Simple circular buffer for staging data before writing to disk
 *  
 *  author      hugo benichi
 *  email       hugo.benichi@m4x.org
 *  copyright   2012 hugo benichi  
 *  version     0.1.1
 *
 */
 

#include <stdlib.h>
#include <stdio.h>
#include <c_buffer.h>
#include <assert.h>


DLL cbuffer*
  cbuffer_new(
    int chunk,      // the size in byte of one chunk of buffer memory
    int size        // the total size in byte of the buffer, at creation
  )
{
  assert(chunk > 0);
  assert(size > 0);
 
  int n = size / chunk;
  if (!n) n = 2;
  
  link*  lnk_ary = malloc( n * sizeof(link) );  
      
  if ( !lnk_ary )
    fprintf(stderr,"cbuffer.c: error while allocating memory for new cbuffer\n");

  int i = 0;
  while( n - i)
  {
    lnk_ary[i].mem = malloc(chunk);
    if( !( lnk_ary[i].mem ) )
      fprintf(stderr,"cbuffer.c: error while allocating memory for new cbuffer\n");
    if(i+1 < n) 
      lnk_ary[i].next = &lnk_ary[i+1];
    i++;
  } 
  lnk_ary[n-1].next = lnk_ary;    
      
      
  cbuffer* cbuf = malloc(sizeof(cbuffer));
  if ( !cbuf )
    fprintf(stderr,"cbuffer.c: error while allocating memory for new cbuffer\n");  

  cbuf->inbound   = lnk_ary;      // start feeding at mem[0]
  //cbuf->start     = lnk_ary;      // remember mem[0]
  cbuf->start     = NULL;      // remember mem[0]
  cbuf->outbound  = NULL;         // null until inbound != start
  cbuf->stop      = NULL;         // when not null inbound as stopped at stop
  cbuf->output    = NULL;
  cbuf->chunk     = chunk;

  return cbuf;
}


DLL void    
  cbuffer_free(cbuffer* buf)
{
  link* l = buf->start;
  link* s = buf->start;
  while( l != s)
  {
    free(l->mem);
    l = l->next;
  }
  free( buf->start );
  free( buf );
}


DLL int   
  cbuffer_open(
    cbuffer*      buf, 
    const char*   address
  )
{
  buf-> output = fopen(address, "w" );
  
  if( !(buf->output) )
  {
    fprintf(stderr, "cbuffer.c: error while opening file %s\n", address);
    return -1;
  }
   
  return 0;  
}


DLL char*
  cbuffer_next(cbuffer* buf)
{
  if ( buf->start  ) // wait until the next buffer is free
  {
    while( buf->inbound->next == buf->outbound ); //busy wait until the next buffer is free
    buf->inbound = buf->inbound->next;  
  } else { 
    buf->start = buf->inbound;  
  }
  return buf->inbound->mem;
}  

  
DLL char*
  cbuffer_has_next(cbuffer* buf)
{
  char* out = NULL;
  if ( buf->start  )
  { 
    if( buf->inbound != buf->outbound )
    {
      buf->inbound = buf->inbound->next;
      out = buf->inbound->mem;
    }
  } else {
    buf->start = buf->inbound; 
    out = buf->inbound->mem;
  }
  return out;
}

  
DLL void
  cbuffer_stop(cbuffer* buf) 
{
  buf->stop = buf->inbound;
  buf->inbound = buf->inbound->next; 
}

  
DLL int
  cbuffer_write(cbuffer* buf)
{  
  while( !( buf->start ) || buf->inbound == buf->start); 
  // busy wait while the first buffer is written

  buf->outbound = buf->start;
  
  int count = 0;
  while( !( buf->stop ) )
  { 
    fwrite( buf->outbound->mem, 1, buf-> chunk, buf-> output);
    count += buf->chunk;

    buf->outbound = buf->outbound->next;
    while(buf->outbound == buf->inbound); //busy wait
  }
  
  while(1)
  { 
    fwrite( buf->outbound->mem, 1, buf-> chunk, buf-> output);
    count += buf->chunk;
    if( buf->stop == buf->outbound )
      break;
    buf->outbound = buf->outbound->next;
  }
   
  fclose(buf->output);
  return count;
}


DLL int
  cbuffer_read(cbuffer* buf, int (*reader)(char*, void*) , void* context)
{  
  while( !( buf->start ) || buf->inbound == buf->start); 
  // busy wait while the first buffer is written

  buf->outbound = buf->start;
  
  int count = 0;
  while( !( buf->stop ) )
  { 
    reader( buf->outbound->mem, context);
    
    count += buf->chunk;

    buf->outbound = buf->outbound->next;
    while(buf->outbound == buf->inbound); //busy wait
  }
  
  while(1)
  { 
    reader( buf->outbound->mem, context);
    count += buf->chunk;
    if( buf->stop == buf->outbound )
      break;
    buf->outbound = buf->outbound->next;
  }
   
  return count;
}
