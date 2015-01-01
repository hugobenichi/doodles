module Sort

  def sorted?
    x = self.min
    self.inject(true) do |sorted,y| 
      sorted &&= x <= y
      x = y
      sorted
    end    
  end

  #index version of selection sort

  def sort_select
    l = self.length
    [].tap do |sorted|
      self.each_index do |i|
        j = i
        (i..l-1).each{ |k| j = k if self[k] < self[j] }      
        sorted << self[j]
        self[j] = self[i]
      end
    end
  end

  def sort_select!
    l = self.length
    self.each_index do |i|
      j = i
      (i..l-1).each{ |k| j = k if self[k] < self[j] }      
      swap = self[j]
      self[j] = self[i]
      self[i] = swap
    end
    self
  end

end


class Array
  include Sort
end

n = 10
a = Array.new(n) { rand 256 }
a_order = a.join(" ")
b = a.sort_select
puts  a_order, a.sorted?, 
      b.join(" "), b.sorted?

a.sort_select!
puts  a.join(" "), a.sorted? 

puts a
