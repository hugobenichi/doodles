# Author:       Hugo Benichi <hugo.benichi@m4x.org>
#
# Copyright:    Copyright (c) 2012 Hugo Benichi
#               You can redistribute and/or modify without restriction.
#
# Description:  creates a Monitor object that generate random data to be plotted
#               the data is plotted 'in real time' thanks to the callback interface to RedPlot
#               

require 'redplot'

class Monitor
  include RedPlot
  attr_accessor :span, :delta
  def initialize
    @times, @span, @delta = [0], 50, 0.1
  end
  def add_channel(n=1)
    n.times do |i|
      data = [i+rand]
      plot.todraw.add('w l title "channel #%d"'% (i+1) ) do #add the callback here
        data << i+rand                                      #generate new data in the callback
        [ @times[-@span..-1] || @times , data[-@span..-1] || data ]
      end   
    end 
    self
  end
  def start
    action = Thread.new do
      puts "Monitor #{self}: starting"
      while @times[-1] < 200
        @times << @times[-1] + 1
        plot.draw
        sleep @delta
      end
      puts "Monitor #{self}: stopping"
    end
    puts "MONITOR: press any key to stop activity"
    STDIN.gets
    action.terminate
  end
end


mon = Monitor.new.add_channel(4).start


