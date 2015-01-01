#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "./data_struct/list.h"

void ary_print(int* ary, int l)
{
  int i = 0;
  while(l-i) printf("%i ",ary[i++]);
  printf("\n");
}


void ary_rand(int* ary, int l, int mod)
{
  int i = 0;
  while(l-i) ary[i++] = rand() % mod;
}

void ary_merge_sort
  (int* in, int* out, int start, int q_stop, int p_stop  ) //assuming p = q_stop
{
  int i = 0, q = start, p = q_stop;
  while(1)
  {
                               //printf("index %i  |  comparing %i@%i and %i@%i",i,ary[q], q,ary[p], p);
    if ( in[q] < in[p] ) 
    {
                                //printf("  | choosing %i@%i\n",ary[q], q);
      out[i++] = in[q++];
      if (q == q_stop)
      {
        while(p < p_stop) out[i++] = in[p++];
        break;
      }
    } else {
                                //printf("  | choosing %i@%i\n",ary[p], p);
      out[i++] = in[p++];
      if (p == p_stop)
      {
        while(q < q_stop) out[i++] = in[q++];
        break;
      }
    }
  }
  memcpy((void*)(in+start), (void*)out, (p_stop-start)*sizeof(int));
}

void tabprintf
  (int a, int b, int c)
{
  int i = 0;
  while(c-i)
  {
    if (i == a) {
      printf("%i ",a);
    } else {
      if (i == b) {printf("%i ",b); } else {printf("  ");}
    }
    i++;
  }
  printf("%i\n",c);
}

void merge_sort(int* in, int* out, int start, int end)
{
  if (start == end)
    return;
  if (start + 1 == end) 
    return;

    int pivot = (end+start)/2;
    tabprintf(start,pivot,end);
    merge_sort(in,out,start,pivot);
    merge_sort(in,out,pivot,end);
    ary_print(in,10);
    ary_merge_sort(in,out,start,pivot,end);
}

int main(void) {

  srand ( time(NULL) );

  int n = 10;
  //int ary[10] = {4,56,2,78,1,5,3,45,15,20};
  int in[10]  = {1,2,4,56,78,3,5,15,20,45};
  int out[10] = {0};
  ary_rand(in,n,100);

  llist* lin = ary2llist(in,);


  ary_print(in,n);
  //ary_merge_sort(in,out,0,5,10);
  //ary_merge_sort(in,out,0,1,2);
  merge_sort(in,out,0,n);
  ary_print(in,n);

  return 0;
}
