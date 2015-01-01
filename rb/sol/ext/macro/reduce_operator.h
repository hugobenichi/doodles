


/*
 *  reduce operations for arity 1
 *
 */

#define accum_plus(X,Y)     Y += X
#define accum_time(X,Y)     Y *= X
#define accum_l1(X,Y)       Y += ABS(X)    //needs the ABS macro
#define accum_l2(X,Y)       Y += (X)*(X)   //unnormed

#define select_min(X,Y)     if( X < Y) Y = X
#define select_max(X,Y)     if( X > Y) Y = X


/*
 *  reduce operations for arity 2
 *
 */

#define accum_2plus(X,Y,Z)    Z += X + Y
#define accum_2minus(X,Y,Z)   Z += X - Y
#define accum_dot(X,Y,Z)      Z += (X) * (Y)   //this is dot product
#define accum_idot(X,Y,Z)     Z += (X) / (Y)

#define accum_dist_l1(X,Y,Z)  Z += ABS(X-Y)    
#define accum_dist_l2(X,Y,Z)  Z += (X-Y) * (X-Y)  //unnormed




