struct ArrayFifo;

typedef struct ArrayFifo fifo;

int fifo_new(fifo** address, int depth, int block_bytes);
void fifo_free(fifo* f);

int fifo_full(fifo* f);
int fifo_empty(fifo* f);

int fifo_push(fifo* f, void* object);
void* fifo_pop(fifo* f);

int fifo_write_to();
