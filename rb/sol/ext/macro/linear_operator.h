

////      ARITY 1


/*
 * all special functions
 *  exp, log, log2, log10
 *  cos, sin, tan,acos, asin, atan
 *  cosh, sinh, tanh, acosh, asinh, atanh
 *  erf
 */

#define spec_exp(X,Y)        Y = exp(X)
#define spec_log(X,Y)        Y = log(X)
#define spec_log2(X,Y)       Y = log2(X)
#define spec_log10(X,Y)      Y = log10(X)
#define spec_cos(X,Y)        Y = cos(X)
#define spec_sin(X,Y)        Y = sin(X)
#define spec_tan(X,Y)        Y = tan(X)
#define spec_acos(X,Y)       Y = acos(X)
#define spec_asin(X,Y)       Y = asin(X)
#define spec_atan(X,Y)       Y = atan(X)
#define spec_cosh(X,Y)       Y = cosh(X)
#define spec_sinh(X,Y)       Y = sinh(X)
#define spec_tanh(X,Y)       Y = tanh(X)
#define spec_acosh(X,Y)      Y = acosh(X)
#define spec_asinh(X,Y)      Y = asinh(X)
#define spec_atanh(X,Y)      Y = atanh(X)
#define spec_erf(X,Y)        Y = erf(X)


/*
 * some power functions:  ^2, ^3, ^1/2, 1/^1/2
 */

#define pow_sqr(X,Y)          Y = (X)*(X)
#define pow_isqr(X,Y)         Y = 1.0 / ( (X)*(X) )
#define pow_sqrt(X,Y)         Y = sqrt(X)
#define pow_isqrt(X,Y)        Y = 1.0/sqrt(X)
#define pow_cub(X,Y)          Y = (X)*(X)*(X)
#define pow_icub(X,Y)         Y = 1.0 / ( (X)*(X)*(X) )


/*
 * other
 */

#define othr_inv(X,Y)         Y = 1.0/(X)
#define othr_abs(X,Y)         Y = ABS(X)
#define othr_upper(X,Y)       if (X > ZERO) Y = X
#define othr_lower(X,Y)       if (X < ZERO) Y = X


///// ARITY 2


/*
 * algebra
 */

#define alg_plus(X,Y,Z)       Z = (X)+(Y)     
#define alg_minus(X,Y,Z)      Z = (X)-(Y)    
#define alg_time(X,Y,Z)       Z = (X)*(Y)    
#define alg_div(X,Y,Z)        Z = (X)/(Y)    
#define alg_pow(X,Y,Z)        Z = pow(X,Y)  

  
