#include <stdio.h>
#include <stdlib.h>

static char hex_rep[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

void to_hex(int value, char* buffer) {
    *(buffer)   = hex_rep[(value & 0xf0) >> 4];
    *(buffer+1) = hex_rep[value & 0xf];
}

char* format_rgb(int x){
    char* format_string = malloc(sizeof(char)*(9));
    if (!format_string) {
        fprintf(stderr, "failed malloc at line __LINE__ in file " __FILE__ "\n");
        exit(1);
    }
    format_string[0] = '0';
    format_string[1] = 'x';
    format_string[8] = '\0';
    to_hex((x & 0xff0000) >> 16, format_string+2);
    to_hex((x & 0x00ff00) >> 8, format_string+4);
    to_hex(x & 0x0000ff, format_string+6);
    return format_string;
}

void print_table(int max) {
    for (int row = 1; row <= max; ++row) {
        for (int col = 1; col <= max; ++col) {
            printf("%4d", row*col);
        }
        printf("\n\n");
    }
}

int fib(int n) {
    if (n <= 1) return n;
    int a = 1;
    int b = 0;
    while(--n){
        a = a + b;
        b = a - b;
    }
    return a;
}


int main(int argc, char* argv[]){
/*
    print_table(10);

    int n = 5;
    if (argc > 1) n = atoi(argv[1]);
    printf("%i\n", fib(n));
*/
#define HEX_NUMBER      0x34a2d5

    printf( "%d in format_rgb: %s\n", HEX_NUMBER, format_rgb(HEX_NUMBER) );

    exit(0);
}
