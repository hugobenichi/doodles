#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int bin_search_it(int* ary, int len, int val);
int bin_search_rec(int* ary, int from, int to, int val);

int bin_search(int* ary, int len, int val) {
//    return bin_search_it(ary, len, val); 
    return bin_search_rec(ary, 0, len-1, val);
}

int bin_search_it(int* ary, int len, int val) {

    int from = 0, to = len-1;
    int pivot;
    int piv_val;

    while (to >= from) {

        pivot = (to+from)/2;
        piv_val = ary[pivot];

        if (piv_val == val) return pivot;

        if (piv_val > val)
            to = pivot-1;
        else
            from = pivot+1;
    }
    return -1;
}

int bin_search_rec(int* ary, int from, int to, int val) {
    if (to-from <= 0) return (ary[from] == val ) ? from : -1;

    int pivot = (to+from)/2;
    int piv_val = ary[pivot];
    if (piv_val == val) return pivot;

    if (piv_val > val)  return bin_search_rec(ary, from, pivot-1, val);

                        return bin_search_rec(ary, pivot+1, to, val);
}

int fill_array(int* ary, int len) {
    int x = rand()%5;
    for(int i = 0; i != len; ++i) {
        ary[i] = x;
        x += rand()%5+1;
    }
    return x;
}

void shuffle(int* ary, int len){
    int last = len-1;
    while (last > 1) {
        int rand_index = rand()%len;
        int swap = ary[rand_index];
        ary[rand_index] = ary[last];
        ary[last] = swap;
        last--;
    }
}

void print_ary(int* ary, int len) {
    for (int i = 0; i != len; ++i) 
        printf("%i ", ary[i]);
    printf("\n");
}

int main(int argc, char* argv[]) {
    printf("%li\n", time(NULL));
    srand(time(NULL));
    int len = 10;
    int *ary = malloc(sizeof(int)*len);
    int biggest = fill_array(ary, len);
    int look_for = rand()%len;
//    if (argc > 1)
//        look_for = atoi(argv[1])%len;
    printf("\nlooking for %i at index %i in arry \n", ary[look_for], look_for);
    shuffle(ary, len);
    print_ary(ary, len);
    int found = bin_search(ary, len, ary[look_for]);
    if (found == -1)
        printf("not found\n");
    else 
        printf("found ary[%i] = %i at index %i\n", found, ary[found], found);

    free(ary);
    exit(0);
}
