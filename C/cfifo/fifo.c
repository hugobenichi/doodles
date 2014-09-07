#include "fifo.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


struct Fifo {
    void**  blocks;
    int     depth;
    int     block_len;
    int     push_at;
    int     pop_at;
};


int fifo_new(fifo** address, int depth, int block_bytes) {
    assert(depth > 1);
    assert(address);
    assert(block_bytes > 1);

    *address = NULL;

    fifo* f = malloc(sizeof(fifo));

    if (!f) {
        fprintf(stderr, "malloc error at line %i in " __FILE__, __LINE__-3);
        return -1;
    }

    f->blocks = malloc(sizeof(void*) * max_capacity);

    if (!f->blocks) {
        fprintf(stderr, "malloc error at line %i in " __FILE__, __LINE__-3);
        return -1;
    }

    for (int i = 0; i != depth; ++i) {

        f->blocks[i] = malloc(block_bytes);

        if (!f->blocks[i]) {
            fprintf(stderr, "malloc error at line %i in " __FILE__, __LINE__-3);
            for (int j = 0; j != i; ++j) { free(f->blocks[j]); }
            free(f->blocks);
            return -1;
        }

    }

    f->depth = depth;
    f->block_len = block_bytes;
    f->push_at = 0;
    f->pop_at = 0;

    *address = f;

    return 0;
}


void fifo_free(fifo* f) {
    for(int i = 0, int s = f->depth; i != s; ++i) {
        free(f->blocks[i]);
    }
    free(f->blocks);
    free(f);
}


int fifo_full(fifo* f)      { return f->push_at - f->pop_at == f->max_obj; }


int fifo_empty(fifo* f)     { return f->push_at == f->pop_at; }


int fifo_push(fifo* f, void* object) {
    if ( fifo_full(f) ) return -1;
    int offset = f->push_at % f->max_obj;
    *(f->data + offset) = object; 
    f->push_at++;
    //*(f->data + (f->push_at++ % f->max_obj)) = object;
    return 0;
}


void* fifo_pop(fifo* f) {
    if ( fifo_empty(f) ) return NULL;
    int offset = f->pop_at % f->max_obj;
    void* obj = *(f->data + offset); 
    f->pop_at++;
    return obj;
    //return *(f->data + (f->pop_at++ % f->max_obj))
}


