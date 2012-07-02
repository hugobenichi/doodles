#include <stdio.h>
#include <stdlib.h>

void print_join(int* ary, int len)
{
  long mult = 1;
  long rez = 0;
  int i = 0;
  for(;i < len; i++, mult *= 10)
    rez += mult * ary[i];
  printf("%li\n",rez);
}

int main(void)
{

  //#define print_join(x) printf("%.1i%.1i%.1i%.1i%.1i%.1i%.1i%.1i%.1i%.1i\n",\
  x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9])

  int x[10] = {9,8,7,6,5,4,3,2,1,0};

print_join(x,10);

  int lim = 280;//0000;
  int order = 1;
  int first = 0;
  
  
  while(order < lim)
  {
  print_join(x,10);
    int i = first;
    int j = first+1;
    while( x[i] < x[j] && j < 10)
      j++;
 // and need to check all pair in between arghs !!
/*
  for example
  
  x = 234567891
    234567819 > x ? no i = 1
    234567189 > x ? no i = 2
      234567981 > x ? yes
  
  x = 234567981   
    234567918 > x ? no  i = 1
    234567198 > x ? no  i = 2
      234567891 > x ? no j = 1 < 2
    234561798 > x ? no  i = 3
      234568971 > x ? yes j = 1 < 3
      234569781 > x ? yes j = 2 < 3
    
*/       
    if( j < 10)
    {
    printf("pivot %i\n", j);
      int k, y = x[i];
      for( k = i; k < j; k++)
        x[k] = x[k+1];
      x[j] = y;
      order++;
      first = 0;
      //if( !(order % 100)) {printf("%i:   ",order); print_join(x,10);}
    } else {first++;}
  }
  
  print_join(x,10);

  exit(0);
}
