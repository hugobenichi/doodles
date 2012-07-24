#!/usr/local/bin/ruby

lim = 200000 #this generates a big enough string

all = (1..lim).each.inject("0"){ |s,x| s << x.to_s }

puts [1e0,1e1,1e2,1e3,1e4,1e5,1e6].map{ |i| all[i.to_i].to_i }.inject(1){|p,x|p *= x}
