#!/usr/local/bin/ruby


class Array
  include Comparable
  def <=> ary
    i = 0
    rez = 0
    while self[i]
      rez = self[i] <=> ary[i]
      break if rez != 0
      i +=1
    end
    return rez
  end

  def next_perm
#puts "current: %s" % self.join

    old = self
    tail = []
    while 1 

      new = Array.new old
      poz = old.length - 1
      while new <= old && poz > -old.length
        poz -= 1
        new = Array.new old
        new.insert poz, new.pop
#puts "trying %s" % new.join
      end

      if poz == -old.length
        tail << old[-1]
        old = old[0..-2]
      else
        break
      end

    end

    new + tail.reverse

  end

end

#x = [0,1,2,3,4]
#30.times{puts ( x = x.next_perm ).join, "" }

if 1 #false
x = [0,1,2,3,4,5,6,7,8,9]

1000000.times{ x = x.next_perm }

puts x
end
