#!/usr/local/bin/ruby

lim = 1000

#square_roots = (0..2*lim*lim).to_a.map{ |x| Math.sqrt x}

square_roots = []

(1..lim/2).each{ |x| square_roots[x*x] = x }


define_singleton_method :find_all_triangles do |p|
  all = 0
  (1..p/2).each do |b|
    bb = b*b
    (1..b).each do |c|  
      bbcc = bb + c*c
      all += 1 if square_roots[bbcc] && p == square_roots[bbcc] + b + c        
    end
  end
  all
end

best_p = 1
best_a = 0

(1..lim).each do |p|
  all = find_all_triangles p
  if all > best_a
    best_a = all
    best_p = p
  end
end

puts best_p, best_a
