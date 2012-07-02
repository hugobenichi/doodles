#!/usr/local/bin/ruby

class Fixnum
  def inv_cycle
#print "1/%i: " % self 
    len = 1
    dig = 0
    ret = 1
    ret *= 10 while ret < self
#print "from: %i " % ret
    mes = [10]
    while 1
      dig = ret / self
      ret = 10*( ret - dig*self )
      break if ret == 0
      while ret < self
        ret *= 10
        len += 1
      end
      
      
#print "%i" % dig
      if mes.include? ret
        break
      else
        mes << ret
        len += 1
      end
    end
#puts " | len: %i" % len
    len
  end
end

max = 1000

longest_c = 1
longest_d = 1

(2..max).each do |i|
  l = i.inv_cycle
  if l > longest_c
    longest_c = l
    longest_d = i
  end
end

puts longest_c, longest_d
