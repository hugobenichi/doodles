# Author:       Hugo Benichi <hugo.benichi@m4x.org>
#
# Copyright:    Copyright (c) 2012 Hugo Benichi
#               You can redistribute and/or modify without restriction.
#
# Description:  creates ramdom walks and random bridges, then plot them and save to disc
#  

require 'redplot'

module RandomWalk

  def to_walk( steps )
    [0.0].tap { |walk| steps.each { |step| walk << walk[-1] + step } }
  end

  def steps_random( n_steps=100, amplitude=0.1 )
    Array.new(n_steps) { rand < 0.5 ? amplitude : -amplitude }
  end

  def steps_bridge( n_steps=100, amplitude=0.1 )
    (Array.new(n_steps/2){amplitude} + Array.new(n_steps/2){-amplitude}).shuffle!
  end

  def walk_random( n_steps=100, amplitude=0.1 )
    to_walk steps_random(n_steps, amplitude)
  end

  def walk_bridge( n_steps=100, amplitude=0.1 )
    to_walk steps_bridge(n_steps, amplitude)
  end

end


include RandomWalk


steps   = 1000
index   = Array.new(steps+1){|i|i}

plot = proc do |method|
  plotter = RedPlot::Plotter.new path: './redplot-example-%s' % method
  3.times do |i|
    plotter.todraw.add('w l title "random walk #%d"'%(i+1)) do 
      [index, send(method, steps)] 
    end
  end
  plotter.draw
  puts "SCRIPT: waiting 2 second"
  sleep 2
  plotter.save_png
end

plot.call :walk_random
plot.call :walk_bridge


