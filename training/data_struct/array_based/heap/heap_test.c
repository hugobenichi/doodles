#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

int is_sorted(int* ary, int len) {

    int prev = ary[0];
    
    for (int i = 0; i != len; ++i) {
        if (ary[i] < prev) return 0;
        prev = ary[i];
    }
    
    return 1;
}

void print_ary(int* ary, int len) {
    for (int i = 0; i != len; ++i)
        printf("%i ", ary[i]);   
    printf("\n");
}

int main(int argc, char* argv[]) {

    int len = 20;
    if (argc > 1) len = atoi(argv[1]);
    if (len < 1) exit(-1);
 
    int* ary = malloc(sizeof(int)*len);

    heap* h;
    heap_new(&h);

    for (int i = 0; i != len; ++i) {
        ary[i] = rand() % 10000;
//        printf("inserting %i, heap is: ", ary[i]);
        heap_insert(h, ary[i]);
//        print_ary(h->data, h->len);
    }

    printf("sorting ary: ");
    print_ary(ary, len);

    heap_sort(h);
    printf("got: ");
    print_ary(h->data, len);
    if (is_sorted(h->data, len))
        printf("sorted !\n");
    else
        printf("BUUU\n");

    heap_free(h);
    free(ary);
    exit(0);
}
