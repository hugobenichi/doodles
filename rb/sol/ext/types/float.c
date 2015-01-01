#include <math.h>
#include "macro/reduce_operator.h"
#include "macro/linear_operator.h"
#include "macro/loop_template.h"


#define   ABS(targ)       fabs(targ)  
#define   ZERO            0.0


#define   clear(targ)     targ = 0.0
#define   set_pinf(targ)  targ = +INFINITY
#define   set_minf(targ)  targ = -INFINITY


/* reduce operations */

def_reduce_arty1( sum,  float, accum_plus, clear)
def_reduce_arty1( prod, float, accum_time, clear)
def_reduce_arty1( l1,   float, accum_l1,   clear)
def_reduce_arty1( l2,   float, accum_l2,   clear)
def_reduce_arty1( max,  float, select_max, set_minf)
def_reduce_arty1( min,  float, select_min, set_pinf)


def_reduce_arty2( dot,  float, accum_dot,  clear)
def_reduce_arty2( l1,   float, accum_dist_l1, clear)
def_reduce_arty2( l2,   float, accum_dist_l2, clear)


/* linear operations */

def_linear_arty1( exp,  float, spec_exp)
def_linear_arty1( log,  float, spec_log)
def_linear_arty1( log2, float, spec_log2)
def_linear_arty1( log10,float, spec_log10)
def_linear_arty1( cos,  float, spec_cos)
def_linear_arty1( sin,  float, spec_sin)
def_linear_arty1( tan,  float, spec_tan)
def_linear_arty1( acos, float, spec_acos)
def_linear_arty1( asin, float, spec_asin)
def_linear_arty1( atan, float, spec_atan)
def_linear_arty1( cosh, float, spec_cosh)
def_linear_arty1( sinh, float, spec_sinh)
def_linear_arty1( tanh, float, spec_tanh)
def_linear_arty1( acosh,float, spec_acosh)
def_linear_arty1( asinh,float, spec_asinh)
def_linear_arty1( atanh,float, spec_atanh)
def_linear_arty1( erf,  float, spec_erf)

def_linear_arty1( sqr,  float, pow_sqr)
def_linear_arty1( isqr, float, pow_isqr)
def_linear_arty1( sqrt, float, pow_sqrt)
def_linear_arty1( isqrt,float, pow_isqrt)
def_linear_arty1( cub,  float, pow_cub)
def_linear_arty1( icub, float, pow_icub)

def_linear_arty1( inv,  float, othr_inv)
def_linear_arty1( abs,  float, othr_abs)
def_linear_arty1( upper,float, othr_upper)
def_linear_arty1( lower,float, othr_lower)

def_linear_arty2( plus, float, alg_plus)
def_linear_arty2( minus,float, alg_minus)
def_linear_arty2( time, float, alg_time)
def_linear_arty2( div,  float, alg_div)
def_linear_arty2( pow,  float, alg_pow)


/* heterogeneous operations */

def_linear_hete(  plus, float, alg_plus)
def_linear_hete(  minus,float, alg_minus)
def_linear_hete(  time, float, alg_time)
def_linear_hete(  div,  float, alg_div)
def_linear_hete(  pow,  float, alg_pow)



