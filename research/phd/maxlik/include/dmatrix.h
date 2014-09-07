

/* Forward declaration, cf end of this file for struct fields definition */
struct Density_Matrix;

typedef 
    struct Density_Matrix   /* the struct name*/
    density_matrix;         /* a shortcut name*/

typedef
    density_matrix          /* a shortcut name */
    dmatrix;                /* a shorter shortcut */

/* 
    calls system malloc to allocate a new dmatrix object which stores
    (photon_numbers + 1)^2 x 2 double numbers (real and imaginary values)
    writes the pointer value to the allocated memory in a caller provided
    pointer address.
    
    If malloc fail, returns 1 and write NULL in the caller provided pointer
    address, otherwise returns 0;
*/
int dmatrix_new(dmatrix** dmatrix_pointer_address, int photon_number);

/*
    calculate the norm of the dmatrix object found at the memory address
    provided by the caller;
*/
double dmatrix_norm(dmatrix* dmatrix_pointer);

/*
    same as dmatrix_norm, but in addition renormalize the dmatrix object.
    the returned double value is the norm before renormalization;
    If the norm is found to be 0 or Nan, the renormalization is not conducted.
*/
double dmatrix_renorm(dmatrix* dmatrix_pointer);

/*
    multiplies two dmatrix objects together and writes the result in a caller
    provided storage, overwriting any precedent value.
    returns 0 if operation succeeded, or return 1 if the photon_number of the
    operands and left value storage does not match, in which case the
    multiplication is not performed
*/
int dmatrix_multiply(dmatrix* left_value_storage, dmatrix* left_operand, dmatrix* right_operand);

/*
    same as dmatrix_multiply(), but the result of the multiplication is added
    elements by elements to the caller provided left value storage.
*/
int dmatrix_addmultiply(dmatrix* left_value_storage, dmatrix* left_operand, dmatrix* right_operand);

/*
    fills with zeros the dmatrix object found at at the memory address provided
    by the caller.
*/
void dmatrix_fill_zero(dmatrix* dmatrix_pointer);

/*
    fills with 1/(photon_number+1) the diagonal of the dmatrix object found at
    the memory address provided by the caller. Other elements are set to 0. All
    imaginary elements are set to 0.
*/
void dmatrix_fill_ones(dmatrix* dmatrix_pointer);

/*
    transposes the dmatrix object found at at the memory address provided by the
    caller.
*/
void dmatrix_transpose(dmatrix* dmatrix_pointer);



struct Density_Matrix {
    double* data; // length = length(real) + length(imag)
    double* real; // length = (photon_number + 1)^2
    double* imag; // length = (photon_number + 1)^2
    int     size; // (photon_number + 1)
    int     phot; // (photon_number)
};
