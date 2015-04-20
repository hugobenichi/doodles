#include <stdio.h>
#include <stdlib.h>

/*
 *  mathematical explanations
 *
 *  for rank n, the number of new numbers added to the spiral is 
 *    n^2 minus the size of the spiral before = n^2 - (n-2)^2 = 4(n-1)
 *  
 *  the recurrence relation for consecutive numbers on the diagonal is
 *    x(n) = x(n-2) + 4(n-1) - offset
 *  where offset is 6 for the SE diag, 4 for the SW, 2 for the NW and 0 for the NE
 *
 *  to compute the sum just add the partial sum of the 4 diagonals, starting from n = 3, and add 1
 *
 *  you can simplify the partial sum by finding the general expression of x(n)
 *    by summing on the recurrence relation
 *    sum(3 to m,+2)[x(n)] = sum(3 to m,+2) [x(n-2)]  4*sum(3 to m,+2) [n] - (4+offset)*sum(3 to m, +2)
 *    x(m) - x(1) = (m+1)^2 - 4 - (4+offset)*(m-1)/2
 *
 *  then resum this term over m to obtain the direct expression of s (but I got lazy there)
 */

int main(void)
{

  int lim = 1001;
  
  int a = 1, b = 1, c = 1, d = 1;
  
  int i = 1;
  int s = 1;
  
  do {
    i += 2;
    int x = 4*(i-1);
    a += x - 6;
    b += x - 4;
    c += x - 2;
    d += x - 0;
    s += a + b + c + d;
  } while( i < lim);

  printf("%i\n",s);

  exit(0);
}
