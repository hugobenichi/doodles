#include <stdlib.h>
#include <stdio.h>

#include "dmatrix.h"
#include "error_codes.h"

static const int code_success         = general_code_success;
static const int code_argument_error  = general_code_argument_error;
static const int code_memalloc_error  = general_code_memalloc_error;
static const int code_procedure_error = general_code_procedure_error;
static const int code_numeric_error   = general_code_numeric_error;


int dmatrix_new(dmatrix** dmatrix_pointer_address, int photon_number) {

    if ( photon_number <= 0 ) 
        return code_argument_error;

    dmatrix* new_object;
    double*  data;
    size_t   size_of_data = 2 * (photon_number+1) * (photon_number+1) * sizeof(double);

    if ( ! (new_object = malloc(sizeof(dmatrix))) )
        return code_memalloc_error;

    if ( ! (data = malloc(size_of_data)) )
        return code_memalloc_error;

    int imaginary_offset = (photon_number+1) * (photon_number+1);

    new_object->data = data;
    new_object->real = data;
    new_object->imag = data + imaginary_offset;
    new_object->size = photon_number + 1;
    new_object->phot = photon_number;

    *dmatrix_pointer_address = new_object;

    return code_success;

}

double dmatrix_norm(dmatrix* dmatrix_pointer) {
    return 0.0;
}

double dmatrix_renorm(dmatrix* dmatrix_pointer) {
    return 0.0;
}

int dmatrix_multiply(dmatrix* left_value_output, dmatrix* left_operand, dmatrix* right_operand) {
    return 0;
}

void dmatrix_fill_zero(dmatrix* dmatrix_pointer) {
    return;
}

void dmatrix_fill_ones(dmatrix* dmatrix_pointer) {
    return;
}

void dmatrix_transpose(dmatrix* dmatrix_pointer) {
    return;
}

#ifdef DMATRIX_TEST

int main(void){
    exit(0);
}

#endif
