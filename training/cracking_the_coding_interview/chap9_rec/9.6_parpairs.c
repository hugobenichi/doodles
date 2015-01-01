#include <stdio.h>
#include <stdlib.h>

static int total_calls = 0;

void print_parpairs(char* buffer, int pairs, int left, int right) {
    if(2*pairs == left + right) {
        printf("%s",buffer);
        total_calls++;
    } else {
        if(left < pairs){
            buffer[left+right] = '(';
            print_parpairs(buffer, pairs, left+1, right);
        }
        if( right < left ){
            buffer[left+right] = ')';
            print_parpairs(buffer, pairs, left, right+1);
        }
    }
}


int main(int argc, char* argv[]) {

    int pairs = 4;
    if(argc > 1) pairs = atoi(argv[1]);
    
    char* buffer = malloc(sizeof(char)*(pairs*2+2));
    buffer[2*pairs] = '\n';
    buffer[2*pairs+1] = '\0';
    
    print_parpairs(buffer, pairs, 0, 0);
    printf("number of combinations %i\n",total_calls);
    
    free(buffer);

    exit(0);
}
