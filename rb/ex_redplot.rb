#!/usr/local/bin/ruby


# examples of using RbPlot

inst = RbPlot.new


inst.settings do 
    set xlabel "tos"
    set xrange  0..1
    # how to deal with special symbol like pi ?
end

inst.todraw(instance) do
    data
end

inst.plot  # this plot what was sent in todraw

inst.replot # send command replot


