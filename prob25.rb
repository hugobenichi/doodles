#!/usr/local/bin/ruby

max_digits = 1000


a, b = [1], [1]
n = 2
#puts a, b

while b.length < max_digits
  ret = 0
  swp = 0
  b.each_index{ |i|
    swp = b[i] + (a[i] || 0) + ret
    a[i] = b[i] 
    ret = swp / 10
    b[i] = swp % 10
  }
  b << ret if ret != 0
  n +=1
  #puts b.reverse.join
end

puts n, b.reverse.join
