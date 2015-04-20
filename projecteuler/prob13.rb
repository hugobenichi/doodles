#!/usr/local/bin/ruby

ranges = [0..9, 10..19, 20..29, 30..39, 40..49]

data = File.open("./prob13.dat",'r')
        .readlines
        .map{ |string| ranges.map{|rng| string[rng].to_i }.reverse }
        .transpose

last = 0
puts data.map{ |col| 
  x = col.inject(0){ |sum,x| sum += x} + last
  last = x/ 10000000000
  [x, last].join " "
}

#data.each do |c| puts c end
