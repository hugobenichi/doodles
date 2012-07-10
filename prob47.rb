#!/usr/local/bin/ruby

class Fixnum
  def factors primes #doesnt give the exposants
    x = self
    [].tap{ |facts|
      primes.each do |p|
        break if p > x
        if x % p == 0
          facts << p
          x /= p
        end
      end
    }
  end
end


lim = 200000
prm = 4

primes = Array.new(lim){|i| i}.tap{ |ary| 
  ary[0], ary[1] = nil, nil
  ary.each_index{ |i|
    if ary[i]
      m = 2*i 
      while m < lim
        ary[m] = nil 
        m += i
      end
    end
  }
}.select{|i| i}


depth = 0
(2..lim).each do |i|
  depth = (i.factors(primes).length == prm) ? depth + 1 : 0 
  puts "depth 3: %s" % Array.new(3){|j| i-j}.reverse.join(" ") if depth == 3
  if depth == prm
    puts Array.new(prm){|j| i-j}.reverse.join(" ") 
    break
  end  
end
