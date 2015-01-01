#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

#define INIT_CAP 16 /* arbitrary */

/*
 * TODO:
 *  add malloc/realloc error checking.
 *  add input checkign and guard against null.
 */

int heap_grow(heap* h);
void heap_add(heap* h, int x);
void heap_swap_up(heap* h, int index);
void heap_swap_down(heap* h, int index);
void swap(int* ary, int i, int j);

int heap_new(heap** pointer_to_heap_address) {
    heap* h = malloc(sizeof(heap));
    h->len = 0;
    h->capacity = INIT_CAP;
    h->data = malloc(INIT_CAP * sizeof(int));
    memset(h->data, -1, INIT_CAP * sizeof(int));
    *pointer_to_heap_address = h;
    return 0;
}

int heap_free(heap* h) {
    free(h->data);
    free(h);
    return 0;
}

/* use realloc() to copy the data to a bigger array */
int heap_grow(heap* h) {
    h->capacity *= 2;
    h->data = realloc(h->data, sizeof(int)*h->capacity); 
    memset(h->data+h->len, -1, sizeof(int)*(h->capacity-h->len));
    return 0;
}

/* check if enough room before adding, if not call heap_grow */
void heap_add(heap* h, int x) {
    if (h->len+1 == h->capacity) heap_grow(h);
    h->data[h->len] = x;
    h->len++;
}

/* add new element at the end of the heap and then enforces heap property */
void heap_insert(heap* h, int x) {
    heap_add(h, x);
    heap_swap_up(h, h->len-1);
}

/* swap head of heap (max element) with  the end and bubble down that last val */
void heap_sort(heap* h) {
    while (h->len > 0) {
        h->len--;                   /* -1 element in the heap */
        swap(h->data, 0, h->len);   /* do the swapping */
        heap_swap_down(h, 0);       /* reinsert h->data[len-1] at correct pos */
    }
}

/* bubble up the parent chain until the heap property is restored */
void heap_swap_up(heap* h, int index) {
    if (index < 0) return;
    int parent = (index-1)/2;
    if (h->data[parent] < h->data[index]) {
        swap(h->data, parent, index);
        heap_swap_up(h, parent);
    }
}

/* bubble down the children chain until the heap property is restored */
void heap_swap_down(heap* h, int index) {
    int left = 2*index + 1;
    int right = left + 1;

    if (right > h->len) return;
    if (right == h->len) right = left;

    int* ary = h->data;
    int swap_with = left;

    if (ary[left] > ary[index] || ary[right] > ary[index]) {
        if (ary[left] < ary[right])
            swap_with = right;
        swap(ary, index, swap_with);
        heap_swap_down(h, swap_with);
    }

}

void swap(int* ary, int i, int j) {
    int s = ary[i];
    ary[i] = ary[j];
    ary[j] = s;
}
