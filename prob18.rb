#!/usr/local/bin/ruby

triangle = File.open("./prob18.dat",'r').readlines.map{|l| l.split(" ").map{|s| s.to_i} }

triangle.each{ |l| puts l.join " " }

all_routes = []

def look ary, i, j, sum, routes
  if ary[i]
    look ary, i+1, j, sum + ary[i][j], routes
    look ary, i+1, j+1, sum + ary[i][j], routes    
  else
    routes << sum
  end
end


all_routes = look triangle, 0, 0, 0, []

puts all_routes.length, all_routes.max
