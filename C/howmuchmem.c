#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  int pow2 = 0;
  while (malloc(1 << pow2)) {
    printf("allocated 2 ^ %d bytes\n", pow2);
    pow2++;
  }
}
