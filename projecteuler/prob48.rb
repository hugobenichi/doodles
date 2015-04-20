#!/usr/local/bin/ruby


lim = 1000
dig = 10

sum = Array.new(dig){0}

Array.new(lim+1) {|i| i 
  x = i
  power = Array.new digits = Array.new(dig) { |j|
    d = x % 10 
    x /= 10
    d
  }
  (i-1).times do 
    ret = 0
    digits.each_index do |j|
      ret = ret + i * power[j]
      power[j] = ret % 10
      ret /= 10
    end
  end
  
  ret = 0
  sum.each_index { |i|
    ret += sum[i] + power[i]
    sum[i] = ret % 10
    ret /= 10
  }
  
} #.map{ |dig| dig.reverse.join "," }

puts sum.reverse.join
