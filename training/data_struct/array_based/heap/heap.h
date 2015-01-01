
struct Heap;

typedef struct Heap heap;

/* create new heap struct with default capacity */
int heap_new(heap** pointer_to_heap_address);

/* free memory used by a heap struct */
int heap_free(heap* h);

/* insert an element into h -> O(log(n)) time with n the size of the heap */
void heap_insert(heap* h, int x);

/* sort the heap from min to max -> O(n log(n)) time */
void heap_sort(heap* h);

struct Heap {
    int* data;
    int  len;
    int capacity;
};
