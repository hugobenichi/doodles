#include <stdio.h>
#include <stdlib.h>

static const char hex[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

void print_low_hex(long val) {
    char buffer[] = "0x00000000";

    int i = 8;
    while(i--) {
        buffer[i+2] = hex[(val & 0xf)];
        val = val >> 4;
    }

    printf("%s", buffer);
}

void print_low_bin(long val) {
    char buffer[] = "0x00000000000000000000000000000000";

    int i = 32;
    while(i--) {
        buffer[i+2] = hex[(val & 1)];
        val = val >> 1;
    }

    printf("%s", buffer);
}

int main(void) {

    int times = 10;
    size_t l_max = 1<<16;
    size_t alignement = 64;

    void* p_ary[times]; // = malloc(sizeof(void*) * times);

    for (int i = 0; i < times; ++i) {
        size_t p_size = rand() % l_max;
        void* p;
//        p = malloc(p_size);
        posix_memalign(&p, alignement, p_size);
        print_low_hex((long) p);
        printf(" ");
        print_low_bin((long) p);
        printf("\n");
        p_ary[i] = p;
    }

    for (int i = 0; i < times; ++i) {
        free(p_ary[i]);
    }

    exit(0);
}

/*
 * use with grep to find pointers which are not aligned:
 *   see_pointer | grep "0$"        # all pointers ending in 0
 *   see_pointer | grep "00$"       # all pointers ending in 00
 *   see_pointer | grep "[^0]$"     # all pointers not ending in 0
 *   see_pointer | grep "[^0]0$"    # all pointers ending in 0 but not in 00
 */
