#include <stdio.h>
#include <stdlib.h>


int hw(int a){
    printf("hello from C -> a = %i\n", a);
    return 42;
}

void write(int *into){
    *into = 42;
}

void read(int *from, int upto){
    for(int i = 0; i < upto; i++) {
        printf("%i: %i\n", i, from[i]);
        from[i] = 1;
    }
}
