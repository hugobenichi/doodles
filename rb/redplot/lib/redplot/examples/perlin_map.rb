# Author:       Hugo Benichi <hugo.benichi@m4x.org>
#
# Copyright:    Copyright (c) 2012 Hugo Benichi
#               You can redistribute and/or modify without restriction.
#
# Description:  draw a landscape map made with Perlin noise
#               

require 'redplot'

module PerlinGrid

  def rand_val(len=10,amplitude=1.0)
    Array.new(len){ amplitude * rand }
  end

  def pad_val(ary, sublength)
    section = sublength / ary.size
    Array.new(sublength) { |i| ary[i/section] }[0..section*ary.size-1]
  end

  def rand_grid(len=10,amplitude=1.0)
    Array.new(len) { Array.new(len){ amplitude * rand } }
  end

  def pad_grid(grid, sublength)
    grid.map! { |col| pad_val col, sublength }
    g = grid.transpose
    g.map! { |col| pad_val col, sublength }
  end

  def add(grid_a, grid_b)
    return grid_b unless grid_a
    grid_a.each_index do |i|
      grid_a[i].each_index do |j|
        grid_a[i][j] += grid_b[i][j]
      end
    end
    grid_a
  end

  def set_zero(grid)
    min = grid.map{|col| col.min }.min
     grid.each_index do |i|
      grid[i].each_index do |j|
        grid[i][j] -= min
      end
    end
    grid 
  end

end


include PerlinGrid


grain = 128
x     = Array.new(grain) { |i| Array.new(grain){ i}<<'' }
y     = Array.new(grain) { Array.new(grain){ |j| j}<<'' }
grid  = [ [2,32],[4,32],[16,32],[32,8],[64,4], ].inject(nil) do | g, param|
          target = add( g, pad_grid( rand_grid( *param ), grain) )
        end.tap do |g| 
          set_zero g
          g.each do |col| col<<'' end
        end


settings  = {
  command:  'splot', 
  path:     './redplot-example-map',
  style:    'with pm3d title "perlin landscape (no smoothing)"'
}


RedPlot::Plotter.new( settings ) do 
  todraw.add( settings[:style] ) { [x.flatten, y.flatten, grid.flatten] }
  set.xrange '[0:%d]' % grain
  set.yrange '[0:%d]' % grain
  set.ticslevel 0
  set.style.line(2).linetype(7).linewidth(0.6)
  set.view '30,60'
  save_png
end


if false  #this code is equivalent to the previous one

  plotter = RedPlot::Plotter.new( settings ) do 
    todraw.add( settings[:style] ) { [x.flatten, y.flatten, grid.flatten] }
  end

  plotter.set.xrange '[0:%d]' % grain
  plotter.set.yrange '[0:%d]' % grain
  plotter.set.ticslevel 0
  plotter.set.style.line(2).linetype(7).linewidth(0.6)
  plotter.set.view '30,60'
  plotter.save_png

end


