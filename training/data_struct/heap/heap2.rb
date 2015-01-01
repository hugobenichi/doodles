class Heap

  attr_accessor :v, :l, :r

  def initialize val
    @v, @l, @r = val, nil, nil
  end

  def maxheapify
    m = self
    [@l, @r].each do |c| m = c if c && c.v > m.v end
    if m != self
      m.v, self.v = self.v, m.v
      m.maxheapify
    end
  end

  def max!
    [@l, @r].each do |c| c.max! if c end
    self.maxheapify
  end

  def max?
    is_max = true
    [@l, @r].each do |c| is_max &= (@v >= c.v) && c.max? if c end
    is_max
  end

  def to_s
    @v.to_s.tap{ |s| 
      s << " " + @l.to_s if @l 
      s << " " + @r.to_s if @r 
    }
  end

  def sorted?
    
  end

  def last depth = 0
    #if @l && @r 
    #  return [ @l.last depth +1, @r.last depth +1 ]  
    #if     
  end

  def sort ary = []
    ary.push self.v
    l = self.last
    if l != self
      self.v = l.v
      l = nil
      self.maxheapify
      self.sort ary
    else
      ary
    end    
  end

  def to_a
    [].from_h self
  end

end

class Array

  def to_h index = 0
    Heap.new(self[index]).tap do |h|
      h.l = self.to_h 2*index+1 if self[2*index+1]
      h.r = self.to_h 2*index+2  if self[2*index+2]
    end
  end

  def from_h h, index = 0
    self[index] = h
    self.from_h(h.l, 2*index+1) if h.l 
    self.from_h(h.r, 2*index+2) if h.r 
    self
  end

  def sorted?
    sorted = true
  end

end

l = 12

a = Array.new(l){rand 32}

puts a.join " "


h = a.to_h

puts h, h.max?
h.max!
puts h, h.max?

b = h.to_a

puts b.inject(""){ |s,c| s << " #{c.v}" } 


