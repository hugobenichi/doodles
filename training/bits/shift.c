#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int x = (-1 << 5);
    int y = (-1 ^ 0x1f);
    printf("%i\n",x);
    printf("%i\n",y);
    exit(0);
}
