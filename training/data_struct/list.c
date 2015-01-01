#include <stdio.h>
#include <stdlib.h>

#include "./list.h"
#include "assert.h"

llist* llist_new(int x) {
  llist* l = malloc(sizeof(llist));
  l-> val = x;
  l-> next = NULL;
  return l;
}

void  llist_free(llist* l) {
  printf("freeing case [%i]\n", l->val);
  if(l->next) {
    llist_free(l->next);
    l->next = NULL;
  }
  free(l->next);
}

int size(llist* l) {
  int s = 1;
  while (l->next) {
    l = l->next;
    s++;
  }
  return s;
}

llist* last(llist* l) {
  while (l->next) {l = l->next;}
  return l;
}

void appen(llist* into, llist* l) {
  last(into)->next = l;
}

void insert(llist* into, llist* l) {
  llist* tail = into->next;
  into->next = l;
  last(l)->next = tail;
}

void add(llist* into, int x) {
  last(into)->next = llist_new(x);
}

llist* head(llist* l) {
  l->next = NULL;
  return l;
}

llist* pop(llist* l) {
  while (l->next->next) {
    l = l->next;  
  }
  return l;
}

llist* ary2llist(int* ary, int size)
{
	assert(size > 0);
	assert(ary);

	llist* l = llist_new(ary[0]);
	llist* p = l;

  int i = 1;
  while(size-i) {
  	p-> next = llist_new(ary[i]);
  	p = p-> next;   
    i++;
  }

  return l;
}

void llistprint (llist* l)
{
  printf("%i ", l->val);
  if(l->next)
    llistprint(l->next);
  else
    printf("\n");
}

void ary_rand(int* ary, int l, int mod)
{
  int i = 0;
  while(l-i) ary[i++] = rand() % mod;
}

void ary_rand_sorted(int* ary, int l, int mod)
{
  ary[0] = rand() % mod;
  int i = 1;
  while(l-i) ary[i++] = ary[i-1] + rand() % mod;
}

llist* sortfuse(llist* l, int l_depth, llist* p, int p_depth)
{
  llist list;
  llist *sorted_last, *sorted;
  sorted = &list;
  sorted->next = NULL;  
  sorted_last = sorted;
  while(1)
  {
    if (l->val < p->val)
    {
      sorted_last->next = l;
      sorted_last = l;
      if(--l_depth)
      {
        l = l->next;
      } else { 
        sorted_last->next = p;
        break;
      }  
    } else {
      sorted_last->next = p;
      sorted_last = p;
      if(--p_depth)
      {
        p = p->next;
      } else { 
        sorted_last->next = l;

        //while(l_depth) {l = l->next; l_depth--;}
        //l->next = NULL;

        break;
      }  
    }
  }
  return sorted->next;
}

void sortmerge(llist* l, int depth)
{
  if (depth == 1)
    return;
  
  int i = 0, pivot = depth / 2;
  llist* p = l;

  while(pivot-i)
  {
    p = p->next;
    i++;
  }
  sortmerge(l,pivot);
  sortmerge(p,depth-pivot);
  sortfuse(l,pivot,p,depth-pivot);
}


/*
int main(void) {

  srand ( time(NULL) );

  int n = 5;
  int in[5]  = {6,7,27,74,90};
  int in2[5]  = {10,12,41,75,82};
  ary_rand_sorted(in,n,20);
  ary_rand_sorted(in2,n,20);
  llist* lin = ary2llist(in,n);
  llist* lin2 = ary2llist(in2,n);


  llistprint(lin);
  llistprint(lin2);
  llistprint( sortfuse(lin,5,lin2,5) );

  printf("\n\n");

  n = 12;
  int in3[12] = {0};
  ary_rand(in3, n, 100);

  llist* l = ary2llist(in3,n);

  llistprint(l);
  sortmerge(l,n);
  llistprint(l);  


  return 0;
}
*/



