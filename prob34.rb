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

i = 3
s = 0
while i < 100000 #only two actually

  if i == i.digits.map{|i| factorials[i] }.inject(0){|sum,x| sum += x}
    puts "found %i" % i
    s += i
  end
  i += 1

end 
