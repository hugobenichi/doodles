/*
 *	a summary file containing a number of bitwise manipulation tricks 
 *  and optimization techniques
 *
 *  warning: use with care, don't forget to test (and plz send me a mail if 
 *  you find something wrong), and put comments on the tricks in your 
 *  code unless you want to torture or/and get hated by your co-workers
 *
 *  copyright hugo benichi 2012  
 *  mail      hugo[dot]benichi[at]m4x[dot]org
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>   // this header file contains definitions of INT32_MAX, INT16_MAX, ...

/* 
 * cast int to an uint32 and then 
 * print all bits starting from the MSB 
 *
 */
void print_bits(int x){
  uint32_t y     =  (uint32_t)x;             // seems like direct conversion works ok
  //uint32_t y     = *( (uint32_t*)  &x  ); 
  uint32_t mask  = 1 << 31;
  
  char string[34]; // char array buffer allow for a single call to printf.
       string[32] = '\n';
       string[33] = '\0';
  char *s = string;
  while(mask) {
  	*s++ = y&mask ? '1' : '0';
    mask = mask >> 1;
  }
  printf("%s",string);
}


/* 
 *  if no arg is given, 'main' prints the
 *  list of bitwise tricks 
 *
 *  else 'main' converts the passed args to
 *  int and prints their bits representation
 *
 */
int main(int argc, char* argv[]){
 
  for ( int i = 1; i < argc; ++i)
  	print_bits( atoi(argv[i]) );
  	
	if ( argc == 1) { 

	printf("all 0               0          ");    print_bits(0);
	printf("all 1              -1          ");    print_bits(-1);
	
	printf("all 0 except LSB    1          ");    print_bits(1);
	printf("all 0 except MSB    INT32_MIN  ");    print_bits(INT32_MIN);
	
	printf("all 1 except LSB   -2          ");    print_bits(-2);
	printf("all 1 except MSB    INT32_MAX  ");    print_bits(INT32_MAX);

	int x = 457800;  
	printf("\nx = %i                     ",x);    print_bits(x); 
	
	// illustrate the relation -x = ~x + 1
	printf("\n~x                             ");  print_bits(~x);
	printf("-x-1                           ");    print_bits(-x-1);
	printf("-x                             ");    print_bits(-x);
	printf("~x+1                           ");    print_bits(~x+1);
	
	printf("\nLSB '1' of x       x&(-x)      ");  print_bits( x&(-x) );
	printf("LSB '1' set to 0   x&(x-1)     ");    print_bits( x&(x-1) );
	printf("LSB '0' of x      ~x&(x+1)     ");    print_bits( ~x & (x + 1) );
	printf("LSB '0' set to 1   x|(x+1)     ");    print_bits( x|(x+1) );
	
	// bit smearing
	int y = x;
	y |= y >> 16; y |= y >> 8; y |= y >> 4; y |= y >> 2; y |= y >> 1;
	int z = y ^ (y>>1);
	printf("\ny = bit_smear(x)               ");  print_bits( y );
	printf("MSB '1' of x       z=y^(y>>1)  ");    print_bits( z );
	printf("MSB '1' set to 0   x&~z        ");    print_bits( x&~z );
	printf("MSB '0' set to 1   x|(z<<1)    ");    print_bits( x|(z<<1) );
	
/*	
	LSB of x:           x & 1
	MSB of x:           x & INT32_MIN
	
	LSB '1' of x        x & (-x)
	MSB ''  of x        
	LSB '0' of x        ~x & (x+1)   	
*/
 
 	}
  	
  exit(0);
}

/* other tricks to include and demonstrate

	swap between a and b
	if (x == a) x= b; else x= a;
	x = a ^ b ^ x


	n & (n-1) == 0    // detects if more than one '1' in n => return true if power of 2


/*
 *	also look at gcc builtin functions 
 *	  http://gcc.gnu.org/onlinedocs/gcc-4.4.2/gcc/Other-Builtins.html
 *		( incidently, gcc __builtin_xxx's provides great optimization 
 *      tools for branches or cache handling, like __builtin_expect 
 *      or __builtin_prefetch )
 *
 *  int __builtin_ffs(uint32_t x):       1 + index of LSB '1' of x
 *	int __builtin_clz(uint32_t x):       number of leading 0
 *	int __builtin_ctz(uint32_t x):       number of trailing 0
 *	int __builtin_popcount(uint32_t x):  number of '1's in x
 *	int __builtin_parity(uint32_t x):    number of '1's modulo 2 in x
 *
 */

