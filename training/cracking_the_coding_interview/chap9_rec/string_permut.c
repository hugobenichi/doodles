#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void swap(char*, char*);
void print_all_permut_rec(char*, char*);

int compchar(const void *p1, const void *p2) {
    return *(char*)p1 - *(char*)p2;
}

void print_all_permut(char* string) {
    qsort(string, strlen(string), sizeof(char), compchar);
    if (string) print_all_permut_rec(string, string);
}


void print_all_permut_rec(char* string, char* offset) {
    if ( *offset == '\0') return;
    if ( *(offset + 1) == '\0') printf("%s\n", string);

    print_all_permut_rec(string, offset+1);

    for ( char* swap_target = offset+1; *swap_target != '\0'; ++swap_target ) {
        if ( *swap_target == *offset ) continue;
        if ( *swap_target == *(swap_target-1)) continue;
        swap(offset, swap_target);
        print_all_permut_rec(string, offset+1);
        swap(offset, swap_target);
    }

}

void swap(char * a_p, char * b_p) {
    char x = *a_p;
    *a_p = *b_p;
    *b_p = x;
}


int main(int argc, char* argv[]) {

    char* string = NULL;
    
    if (argc > 1)
        string = strdup(argv[1]);
    else
        string = strdup("abc");

    print_all_permut(string);

    if (string) free(string);

    exit(0);

}
