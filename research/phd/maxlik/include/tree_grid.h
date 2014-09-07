

struct Tree_Grid_2D;
typedef struct Tree_Grid_2D treegrid_2d;



struct Tree_Node_2D;
typedef struct Tree_Node_2D treenode_2d;



/* return error codes*/
int treegrid_2d_new(treegrid_2d** address_of_pointer, void* something);  //TODO: define necessary args
int treegrid_2d_clean(treegrid_2d* grid);

/* return additional number of cells */
int treegrid_2d_insert(double x, double y, int point_id);

/* return boolean found/not found */
int treegrid_2d_find_node(treenode_2d** address_of_pointer, double x, double y);

/* return error code */
int treenode_2d_new(treenode_2d** address_of_pointer);
int treenode_2d_clean(treenode_2d* node);

/* return additional number of cells */
int treenode_2d_insert(double x, double y, int point_id);

// function for iteration


#define xleft_yleft    0
#define xleft_yright   1
#define xright_yleft   2
#define xright_yright  3


struct Tree_Node_2D {
    int   num_point;        // number of point
    int*  point_indexes;    // array of index of point in client array
    struct Tree_Node_2D* children[4]; // children
};


struct Tree_Grid_2D {
    struct Tree_Node_2D* grid;      // 1d array of pointer, init to null
    double x_offset;                // subtract to x before getting cell index
    double y_offset;                // same for y
    double x_root_cell_size;        // size of x cell, divider to get cell index
    double y_root_cell_size;
    int x_cells;                    // number of x root cells in a row
    int y_cells;                    // number of y root cells in a column
};
