#!/usr/local/bin/ruby

class Fixnum
  def digits
    [].tap{ |digits|
      x = self
      while x != 0
        digits << x % 10
        x /= 10
      end
    }
  end
end

order = 5

pow = (0..9).each.map{|i| i**order}

max = pow[9] * (order+1)
tot = 0
sum = 0

(2..max).each do |i|

  if i == i.digits.inject(0){|s,i| s += pow[i]}
    tot += 1
    sum += i
  end
end

puts tot, sum
