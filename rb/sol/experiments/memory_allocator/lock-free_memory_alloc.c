

/*
 *
 *

  SLIST_ENTRY, 
  InterlockedPushEntrySList, 
  InterlockedPopEntrySList, 
  InitializeSListHead are functions for lock-free single-linked-list operations under Win32. 
  Use the according operations on other OSes.

  Drawbacks :
    Overhead of sizeof( SLIST_ENTRY )
    The buckets can only grow efficiently once at the start.
    After that you can run out of memory and have to ask the OS/other buckets. 
    (Other buckets leads to fragmentation)
    This sample is a bit too easy and must be extended to handle more cases

 *
 *
 */

typedef 
  struct
  {
    union{
      SLIST_ENTRY entry;
      void* list;
    };
    byte mem[];
  } 
  mem_block;

typedef 
  struct
  {
    SLIST_HEADER root;
  } 
  mem_block_list;


#define BUCKET_COUNT          4
#define BLOCKS_TO_ALLOCATE    16

static mem_block_list Buckets[BUCKET_COUNT];

void 
init_buckets
()
{
  for( int i = 0; i < BUCKET_COUNT; ++i )
  {
    InitializeSListHead( &Buckets[i].root );
    for( int j = 0; j < BLOCKS_TO_ALLOCATE; ++j )
    {
      mem_block* p = (mem_block*) malloc( sizeof( mem_block ) + (0x1 << BUCKET_COUNT) * 0x8 );
      InterlockedPushEntrySList( &Buckets[i].root, &p->entry );
    }
  }
}


void* 
balloc
( size_t size )
{
  for( int i = 0; i < BUCKET_COUNT; ++i )
  {
    if( size <= (0x1 << i) * 0x8 )
    {
      mem_block* p = (mem_block*) InterlockedPopEntrySList( &Buckets[i].root );
      p->list = &Buckets[i];
    }
  }
  return 0;   // block to large
}


void  
bfree
( void* p )
{
    mem_block* block = (mem_block*) (((byte*)p) - sizeof( block->entry ));
    InterlockedPushEntrySList( ((mem_block_list*)block)->root, &block->entry );
}
