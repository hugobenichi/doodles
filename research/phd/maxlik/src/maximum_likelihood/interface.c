#include <stdlib.h>
#include <stdio.h>

#include "maximum_likelihood/interface.h"
#include "dmatrix.h"

#include "error_codes.h"
static const int code_success         = general_code_success;
static const int code_argument_error  = general_code_argument_error;
static const int code_memalloc_error  = general_code_memalloc_error;
static const int code_procedure_error = general_code_procedure_error;
static const int code_numeric_error   = general_code_numeric_error;


#define __STRING_LINE__  "00"

//#define errmsg(msg, ...) fprintf(stderr, "error in " __FILE__ " @ line " __STRING_LINE__ ":\n  " msg "\n",  __VA_ARGS__)


#define errmsg(msg, ...) fprintf(stderr, "error in " __FILE__ " @ line %i:\n  " msg "\n",  __LINE__)




static const int min_reasonable_photon_number = 3;
static const int max_reasonable_photon_number = 64;




int validate_procedure(maxlik_proc* strategy);
int initialize_client_context(maxlik_proc* strategy);
int finalize_client_context(maxlik_proc* strategy);

int allocate_memory(int photon_number, dmatrix** resource[3]);

int iterative_mainloop(maxlik_proc* strategy, dmatrix** resource[3]);


int iterative_maximum_likelihood(dmatrix** dmatrix_pointer_address, maxlik_proc* strategy, int photon_number) {

    int return_code = code_success;

    if ( return_code = validate_procedure(strategy) )
        goto exit;

    dmatrix *state = NULL, *operator = NULL, *swap = NULL;
    dmatrix** dmatrix_resources[3] = {&state, &operator, &swap};

    if ( return_code = allocate_memory(photon_number, dmatrix_resources) )
        goto clean_memory;

/* from this point we must clean own allocated memory if error */

    if ( return_code = initialize_client_context(strategy) )
        goto clean_memory;

/* from this point on we must clean client context if error */

    iterative_mainloop(strategy, dmatrix_resources);

finalize:
    finalize_client_context(strategy);

clean_memory:
    if (state)    *dmatrix_pointer_address = state;
    if (operator) free(operator);
    if (swap)     free(swap);

exit:
    return return_code;

}


int iterative_mainloop(maxlik_proc* strategy, dmatrix** dmatrix_resource[3]) {

    dmatrix *state      = *dmatrix_resource[0],
            *operator   = *dmatrix_resource[1],
            *swap       = *dmatrix_resource[2];

    void* context = strategy->context;
    iteration_condition  go_on        = strategy->go_on;
    iteration_operator   get_operator = strategy->get_operator;
    iteration_callback   callback     = strategy->callback;

    dmatrix_fill_ones(state);

    while ( go_on(state, operator, context) ) {

        get_operator(state, operator, context);

        dmatrix_multiply(swap, operator, state);
        dmatrix_multiply(state, swap, operator);
        dmatrix_renorm(state); //TODO: check for nan or 0.0 here

        if( callback ) callback(state, operator, context);

    }

    return code_success;

}


int allocate_memory(int photon_number, dmatrix** dmatrix_resource[3]) {

    dmatrix **p_to_state    = dmatrix_resource[0],
            **p_to_operator = dmatrix_resource[1],
            **p_to_swap     = dmatrix_resource[2];

    if (    photon_number < min_reasonable_photon_number
         || photon_number > max_reasonable_photon_number
    ) {
        errmsg("argument photon_number is not in the reasonable range ");/*[%i:%i]",
                min_reasonable_photon_number
                max_reasonable_photon_number ); */
        return code_argument_error; /* unreasonable client */
    }

    if (    dmatrix_new(p_to_state, photon_number)
         || dmatrix_new(p_to_operator, photon_number)
         || dmatrix_new(p_to_swap, photon_number)
    ) {
        errmsg("memory allocation for dmatrix object failed.");
        return code_memalloc_error; /* malloc error */
    }

    return code_success;  /* no error */

}


int validate_procedure(maxlik_proc* strategy) {

    if ( !strategy ) { 
        errmsg("argument struct Maximum_Likelihood* \"strategy\" is NULL.");
        return code_argument_error;   /* bad, mean client */
    }

    if ( !(strategy->go_on) || !(strategy->get_operator) ) { 
        errmsg("argument struct Maximum_Likelihood* \"strategy\" is not valid: some necessary fields are NULL.");
        return code_argument_error; /* absent-minded client */
    }

    return code_success; /* no error */

}


int initialize_client_context(maxlik_proc* strategy) {

    if ( strategy->initialize ) {
        if( strategy->initialize(strategy->context) ) {
            errmsg("initialization of strategy->context failed");
            return code_procedure_error; /* client failed us >_< */
        }
    }

    return code_success; /* no error */

}


int finalize_client_context(maxlik_proc* strategy) {

    if ( strategy->finalize ) {
        if ( strategy->finalize(strategy->context) ) {
            errmsg("finalization of strategy->context failed");
            return code_procedure_error;  /* client failed us at the very end ! */
        }
    }

    return code_success; /* no error */

}


/* for test */
static int dummy_count = 5;

int dummy(dmatrix* a, dmatrix* b, void* c) { printf("dummy called, returning %i\n", dummy_count); return  dummy_count < 0 ? 0 : dummy_count--; }

struct Int_Wrap {
    int val;
};

typedef struct Int_Wrap int_wrap;

void alloc_int_point(int_wrap **int_w_p, int val) { 
    int_wrap* wrap_p = malloc(sizeof(int_wrap));
    *int_w_p = wrap_p;
    (*int_w_p)->val = val;
}

int main(void) {

/*    // test for array of address of pointer
    int_wrap *a = NULL, *b = NULL;
    int_wrap** ary[2] = {&a, &b};

    alloc_int_point(ary[0], 2);
    alloc_int_point(ary[1], 42);

    if(a == NULL || b == NULL) printf("failed ah ha\n");

    printf("%i\n%i\n", a->val, b->val);
*/

    int phot_num = 5;
    int something;
    int *pointer_to_something = &something;

    maxlik_proc* strategy_p = NULL;
    maxlik_proc strategy;
    strategy.context = NULL;
    strategy.initialize = NULL;
    strategy.finalize = NULL;
    strategy.go_on = dummy;
    strategy.get_operator = dummy;
    strategy.callback = dummy;

    strategy_p = &strategy;

    int ret = iterative_maximum_likelihood((dmatrix**) &pointer_to_something, strategy_p, phot_num);

    if (!ret) printf("ok!\n");

    exit(0);

}
