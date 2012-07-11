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

f = 1
factorials = [1] + (1..9).map{ |i| f *= i }

(3..100000).each do |i| #only two actually
  puts "found %i" % i if i == i.digits.map{|i| factorials[i] }.inject(0){|sum,x| sum += x}
end 
