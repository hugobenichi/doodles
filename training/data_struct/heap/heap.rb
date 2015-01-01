

class Heap

  attr_accessor :val, :left, :right

  def initialize o
    @val, @left, @right = o, nil, nil
  end

  def sort
   
  end

  def to_s
    @val.to_s.tap{ |s| 
      s << " " + @left.to_s if @left 
      s << " " + @right.to_s if @right 
    }
  end

  def max?
    max = true
    [@left, @right].each do |c|
      max &&= @val >= c.val && c.max? if c      
    end
    max
  end

  def max_heapify
    max = self
    [@left,@right].each{ |c| max = c if c && c.val > max.val }
    if max != self  
      max.val, self.val = self.val, max.val
      max.max_heapify        
    end
  end

  def bottom_maxify
    [@left, @right].each do |c| c.bottom_maxify if c end    
    self.max_heapify
  end

  class << self

    def build ary
      ary.to_h
    end

  end

end

class Array

  def to_h index = 0
    if self[index].nil? 
      nil
    else
      x = Heap.new self[index]
      x.left  = self.to_h 2*index+1
      x.right = self.to_h 2*index+2
      x
    end
  end  

end


l = 10
a = Array.new(l) do rand 32 end

h =  a.to_h

puts a.join(" "),""
puts h, h.max?

h.bottom_maxify

puts h, h.max?


