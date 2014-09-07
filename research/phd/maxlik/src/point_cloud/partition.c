#include <stdlib.h>
#include <stdio.h>

#include "point_cloud_partition.h"
#include "tree_grid.h"

#include "error_codes.h"
static const int code_success         = general_code_success;
static const int code_argument_error  = general_code_argument_error;
static const int code_memalloc_error  = general_code_memalloc_error;
static const int code_procedure_error = general_code_procedure_error;
static const int code_numeric_error   = general_code_numeric_error;


int check_data(cloud_2d* data, double* min_max[4]);
int check_specs(specs_2d* specs);
int check_handler(handler_2d* callback);

int insert_all_points(treegrid_2d* grid, cloud_2d* data);


int alloc_output_buffer(double** address_x, double** address_y, int num_cells);

void add_to_output_buffer(cloud_2d* data, treegrid_2d* grid, double* xbuf, double* ybuf);


int find_partition_2d(cloud_2d* data, specs_2d* specs, handler_2d* callback) {

    int return_code = code_success;

    if ( return_code = check_specs(specs) )
        goto exit;

    if ( return_code = check_handler(callback) )
        goto exit;

    double x_min, x_max, y_min, y_max;
    double* min_max[4] = {&x_min, &x_max, &y_min, &y_max};

    if ( return_code = check_data(data, min_max) )
        goto exit;

    treegrid_2d* grid;

    if ( return_code = treegrid_2d_new(&grid, NULL) ) // TODO: define necessary args
        goto exit;

    int num_cells_total = insert_all_points(grid, data);
    if ( num_cells_total < 0) {
        return_code = num_cells_total;
        goto clean_grid;
    }

    double *x_reduc = NULL, *y_reduc = NULL;

    if ( return_code = alloc_output_buffer(&x_reduc, &y_reduc, num_cells_total) )
        goto clean_array;

    add_to_output_buffer(data, grid, x_reduc, y_reduc);

    void* context = callback->use_data_context;
    return_code = callback->use_data(context, x_reduc, y_reduc, num_cells_total);

clean_array:
    if (x_reduc) free(x_reduc);
    if (y_reduc) free(y_reduc);

clean_grid:
    return_code = treegrid_2d_clean(grid);

exit:
    return return_code;

}


int check_specs(specs_2d* specs) {
    return code_success;
}


int check_handler(handler_2d* callback) {
    return code_success;
}


int check_data(cloud_2d* data, double* min_max[4]) {
    return code_success;
}

int insert_all_points(treegrid_2d* grid, cloud_2d* data) {
    // iterate all points in data,
        // insert points
        // in grid
        // count number of cells
    return 1;
}

int alloc_output_buffer(double** address_x, double** address_y, int num_cells) {
    return code_success;
}


void barycentric_mean(double* x, double* y, cloud_2d* data, int* indexes, int indexes_len);

void add_to_output_buffer(cloud_2d* data, treegrid_2d* grid, double* x, double* y) {
    return;
}

void barycentric_mean(double* x, double* y, cloud_2d* data, int* indexes, int indexes_len) {
    return;
}

