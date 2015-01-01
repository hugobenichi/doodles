#include <stdio.h>
#include <stdlib.h>

void bitprint(long value) {
    char buffer[64] = {};
    long mask = 1; 
    for (int i = 0; i != 64; ++i){
        buffer[63-i] = '0';
        if ((value & mask) == mask) buffer[63-i] = '1';
        mask = mask << 1;
    }
    printf("%.64s\n", buffer);
}

int next_two_power(int n) {
    if (n < 0) return 0;
    int y = 0x40000000;
    while ( (y & n) == 0) y >>= 1;
    if (y == n) return y;
    return y << 1;
}

void* malloc_align(size_t bytes, int alignement) {

    if (alignement < 8) alignement = 8;

    int pow = next_two_power(alignement);
    int mask = pow - 1;

printf("alignement: %i\nbit and mask:\n", alignement);
bitprint(pow);
bitprint(mask);
 
    int min_offset = 4;
    if (min_offset < pow/8)
        min_offset = pow/8;

    char* base = malloc(bytes + min_offset);
    if (!base) return NULL;

printf("base pointer:    ");
bitprint((long)base);

    int align = pow - (((long)base) & mask);

    char* aligned_pointer = base + align;
    *( ((int*)aligned_pointer) - 1 ) = align;

printf("aligned pointer: ");
bitprint((long)aligned_pointer);

    return (void*) aligned_pointer;
}

void free_align(void* aligned_pointer) {

    if (!aligned_pointer) return;

    int align = *( ((int*)aligned_pointer) - 1);

    char* offset = ((char*)aligned_pointer) - align;

printf("freeing pointer: ");
bitprint((long)offset);

    free(((char*)aligned_pointer) - align);

}

int main(int argc, char* argv[]) {

    printf("coucou\n");

    int i = 42;
    int* i_p = &i;
    long* l_p = (long*) i_p;

    char* c_p = (char*) i_p;
/*
    bitprint((long)i_p);
    bitprint((long)(i_p+1)); // + 4

    bitprint((long)l_p);
    bitprint((long)(l_p+1)); // + 8

    bitprint((long)c_p);
    bitprint((long)(c_p+1)); // + 8
*/
    int align = 24;

    if (argc > 1)
        align = atoi(argv[1]);

    free_align(malloc_align(1024, align));

    exit(0);
}
