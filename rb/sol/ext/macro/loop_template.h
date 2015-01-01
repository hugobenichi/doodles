#ifndef     SOL_MACRO_LOOP_LAYOUT
  #define   SOL_MACRO_LOOP_LAYOUT


  #define   arty1(TYPE, tag_in, tag_out)\
              (TYPE* tag_in, TYPE* tag_out, int N)


  #define   arty2(TYPE, tag_in1, tag_in2, tag_out)\
              (TYPE* tag_in1, TYPE* tag_in2, TYPE* tag_out, int N)


  /*
   *  First the reduce loops
   *    the output is a pointer to one single memory case (kind of register)
   *    it is first initialized with the INIT macro
   *    then the OPER happens in loop (macro defined seperately)
   *    only inputs are moved
   */


  #define   def_reduce_arty1(NAME,TYPE,OPER,INIT) \
              \
              void sol_ ## TYPE ## _reduce1_ ## NAME arty1(TYPE,in,out)\
              {\
                INIT(*out);\
                while(N--) {\
                  OPER(*in, *out);\
                  in++;\
                }\
              }\


  #define   def_reduce_arty2(NAME,TYPE,OPER,INIT) \
              \
              void sol_ ## TYPE ## _reduce2_ ## NAME arty2(TYPE,in_a,in_b,out)\
              {\
                INIT(*out);\
                while(N--) {\
                  OPER(*in_a, *in_b, *out);\
                  in_a++;\
                  in_b++;\
                }\
              }\


  /*
   *  Second the linear loops
   *    the output is a pointer to another array with good size
   *    it is initialized outside
   *    then the OPER happens in loop (macro defined seperately)
   *    inputs and output are moved
   */


  #define   def_linear_arty1(NAME,TYPE,OPER) \
              \
              void sol_ ## TYPE ## _linear1_ ## NAME arty1(TYPE,in,out) \
              {\
                while(N--) {\
                  OPER(*in, *out);\
                  in++;\
                  out++;\
                }\
              }\


  #define   def_linear_arty2(NAME,TYPE,OPER) \
              \
              void sol_ ## TYPE ## _linear2_ ## NAME arty2(TYPE,in_a,in_b,out) \
              {\
                while(N--) {\
                  OPER(*in_a, *in_b, *out);\
                  out++;\
                  in_a++;\
                  in_b++;\
                }\
              }\


  /*
   * Third the heterogeneous linear|scalar 2 linear loop and linear|scalar 2 scalar loops
   *
   */


  #define   def_reduce_hete(NAME,TYPE,OPER,INIT) \
              \
              void sol_ ## TYPE ## _scalar_hete_ ## NAME arty2(TYPE,in,scalar,out) \
              {\
                INIT(*out);\
                while(N--) {\
                  OPER(*in, *scalar, *out);\
                  in++;\
                }\
              }\


  #define   def_linear_hete(NAME,TYPE,OPER) \
              \
              void sol_ ## TYPE ## _linear_hete_ ## NAME arty2(TYPE,in,scalar,out) \
              {\
                while(N--) {\
                  OPER(*in, *scalar, *out);\
                  out++;\
                  in++;\
                }\
              }\



#endif


/*

#define plus(X,Y)   (X)+(Y)
#define mult(X,Y)   (X)*(Y)

write_function(double, plus)
write_function(double, mult)

*/
