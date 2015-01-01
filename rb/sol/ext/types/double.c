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

def_reduce_arty1( sum,  double, accum_plus, clear)
def_reduce_arty1( prod, double, accum_time, clear)
def_reduce_arty1( l1,   double, accum_l1,   clear)
def_reduce_arty1( l2,   double, accum_l2,   clear)
def_reduce_arty1( max,  double, select_max, set_minf)
def_reduce_arty1( min,  double, select_min, set_pinf)


def_reduce_arty2( dot,  double, accum_dot,  clear)
def_reduce_arty2( l1,   double, accum_dist_l1, clear)
def_reduce_arty2( l2,   double, accum_dist_l2, clear)


/* linear operations */

def_linear_arty1( exp,  double, spec_exp)
def_linear_arty1( log,  double, spec_log)
def_linear_arty1( log2, double, spec_log2)
def_linear_arty1( log10,double, spec_log10)
def_linear_arty1( cos,  double, spec_cos)
def_linear_arty1( sin,  double, spec_sin)
def_linear_arty1( tan,  double, spec_tan)
def_linear_arty1( acos, double, spec_acos)
def_linear_arty1( asin, double, spec_asin)
def_linear_arty1( atan, double, spec_atan)
def_linear_arty1( cosh, double, spec_cosh)
def_linear_arty1( sinh, double, spec_sinh)
def_linear_arty1( tanh, double, spec_tanh)
def_linear_arty1( acosh,double, spec_acosh)
def_linear_arty1( asinh,double, spec_asinh)
def_linear_arty1( atanh,double, spec_atanh)
def_linear_arty1( erf,  double, spec_erf)

def_linear_arty1( sqr,  double, pow_sqr)
def_linear_arty1( isqr, double, pow_isqr)
def_linear_arty1( sqrt, double, pow_sqrt)
def_linear_arty1( isqrt,double, pow_isqrt)
def_linear_arty1( cub,  double, pow_cub)
def_linear_arty1( icub, double, pow_icub)

def_linear_arty1( inv,  double, othr_inv)
def_linear_arty1( abs,  double, othr_abs)
def_linear_arty1( upper,double, othr_upper)
def_linear_arty1( lower,double, othr_lower)

def_linear_arty2( plus, double, alg_plus)
def_linear_arty2( minus,double, alg_minus)
def_linear_arty2( time, double, alg_time)
def_linear_arty2( div,  double, alg_div)
def_linear_arty2( pow,  double, alg_pow)


/* heterogeneous operations */

def_linear_hete(  plus, double, alg_plus)
def_linear_hete(  minus,double, alg_minus)
def_linear_hete(  time, double, alg_time)
def_linear_hete(  div,  double, alg_div)
def_linear_hete(  pow,  double, alg_pow)


