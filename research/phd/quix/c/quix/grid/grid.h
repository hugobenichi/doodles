#ifndef QUIX_GRID
  #define QUIX_GRID


  #include "quix/grid/range.h"

  #define   if_cartesian(grid)        if (grid-> type == cartesian)
  #define   if_radial(grid)           if (grid-> type == radial)
  #define   if_histo(grid)            if (grid-> type == histo)


  typedef 
    enum proto_grid_type
    {
      cartesian, 
      radial,
      histo
    } 
    grid_type;

  typedef 
    struct proto_grid
    {
      range*      x;
      range*      y;
      grid_type   type;
    } 
    grid;


  EXT   grid*       grid_xy         (double, int);
  EXT   grid*       grid_rad        (double, int, int);
  EXT   grid*       grid_copy       (grid*);
  EXT   void        grid_free       (grid*);
  EXT   void        grid_hist       (grid*);
  EXT   double      grid_cell       (grid*);

/*
  EXT   grid*       grid_rotate     (grid*, double);
  EXT   void        grid_dilate     (grid*, range_mod);

  EXT   double*     grid_read_x     (grid*);
  EXT   double*     grid_read_y     (grid*);
  EXT   int         grid_read_x_n   (grid*);
  EXT   int         grid_read_y_n   (grid*);
*/

#endif
