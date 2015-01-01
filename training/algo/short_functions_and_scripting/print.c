#include <stdio.h>
#include <stdlib.h>

/*
 *  print a string without
 *  looping or using local var
 */
void print_rec(char* string) {
    if (*string == '\0') { putchar('\n'); return;}
    putchar(*string);
//    printf("%c", *string);
    print_rec(string+1);
}

/*
 *  print a string in reverse order
 *  without looping or using local var
 */
void print_rev_rec(char *string) {
    if (*string == '\0') return;
    print_rev_rec(string+1);
    putchar(*string);
}

void print_rev(char *string) {
    print_rev_rec(string);
    putchar('\n');
}

int main(int argc, char* argv[]) {

    char charary[] = "abc";

    char *string = charary;
    //string[3] = '\0';

    if (argc > 1) string = argv[1];

    print_rec(string);
    print_rev(string);

}
