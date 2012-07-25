#!/usr/local/bin/ruby

# the brute force approach, but it works lol

rng = (2..100)

puts [].tap{ |nums|
  rng.each{ |a| 
    #x = a
    rng.each{ |b|
      #x *= a
      x = a**b
      nums << x unless nums.include? x  # could be made much better with a binary search tree
    }
  } 
}.length
