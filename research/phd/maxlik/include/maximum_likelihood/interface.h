

struct Maximum_Likelihood_Procedure; // forward declaration, cf end of file for definition

struct Density_Matrix;  // forward declaration, cf dmatrix.h for definition)


/*
    performs a general pattern for iterative maximum likelihood reconstruction.
    the pattern is decided by the maximum_likelihood_procedure 
*/
int iterative_maximum_likelihood(
    struct Density_Matrix**                 dmatrix_pointer_address,
    struct Maximum_Likelihood_Procedure*    strategy,
    int                                     photon_number
);


/* typedef shortcuts for procedure struct */

typedef
    struct Maximum_Likelihood_Procedure
    maximum_likelihood_procedure;

typedef
    maximum_likelihood_procedure
    maxlik_proc;

/* typedef shortcuts for procedure function pointers */

typedef
    int (*resource_handler)(void* resource_pointer);

typedef
    resource_handler
    context_initializer;

typedef
    resource_handler
    context_finalizer;

typedef
    int (*computation_deleguate)(struct Density_Matrix* state, struct Density_Matrix* operator, void* context);

typedef
    computation_deleguate
    iteration_condition;

typedef
    computation_deleguate
    iteration_operator;

typedef
    computation_deleguate
    iteration_callback;


/* main struct definition */

struct Maximum_Likelihood_Procedure {
    void*       context;
    context_initializer     initialize;
    context_finalizer       finalize;
    iteration_condition     go_on;
    iteration_operator      get_operator;
    iteration_callback      callback;
};


