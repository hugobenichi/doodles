
/*
 *  forward declation of struct to pass data to partitioner procedure
 *  cf end of flie for definition
 */

struct Point_Cloud_2D;  /* wraps input data: sequence of (x,y) points */
struct Grid_Specs_2D;   /* gives bounds to min cell size and max cell number */
struct Data_Handler_2D; /* client callback for copying partitioned data */


/* procedure which finds a sequence of points which */
int find_partition_2d(
    struct Point_Cloud_2D*  data,
    struct Grid_Specs_2D*   specs,
    struct Data_Handler_2D* callback
);


typedef struct Point_Cloud_2D   cloud_2d;

typedef struct Grid_Specs_2D    specs_2d;

typedef struct Data_Handler_2D  handler_2d;


struct Point_Cloud_2D {
    double* x_data;
    double* y_data;
    int     num_point;
};


struct Grid_Specs_2D {
    double xcell_max_size;
    double ycell_max_size;
    int    cell_max_num_points;
};


struct Data_Handler_2D {
    int (*use_data)(double* context, double* x_data, double* y_data, int len_data);
    void* use_data_context;
};
