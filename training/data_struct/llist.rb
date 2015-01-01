class LList
  attr_accessor :val, :next  #necessary only because of reverse!

  def initialize x = nil
    @val, @next = x, nil
  end

  def to_s
    self.inject("") { |string, x| string += x.to_s + " " }
  end

  def size s = 1
    @next ? @next.size(s+1) : s  
  end

  def last
    @next ? @next.last : self
  end

  def head
    LList.new @val
  end

  def pop
    return self unless @next
    if @next.next
      @next.pop
    else
      to_pop = @next
      @next = nil
      to_pop  
    end
  end

  def each &block
    block.call(@val)
    @next.each(&block) if @next
  end

  include Enumerable

  # insert a list in @next and return self
  # the reminder is forgotten
  def < llist
    @next = llist if llist.is_a? LList 
    self
  end

  #insert a list at the end of the current list
  def << llist
    self.tap{ |x| x.last < llist }
  end

  #insert a list here and link back the reminder
  def | llist
    self < llist << @next if llist.is_a? LList 
  end

  #add value x at the end of the list in a new listbox
  def add x
    self << LList.new(x)
  end

  #insert value x in place and linkback @next
  def insert x
    self < LList.new(x) < @next
  end

  def insert_order x 
    if x <= @val    
      @next = LList.new(@val) << @next
      @val = x
    else
      @next = @next ? @next.insert_order(x) : LList.new(x)
    end
    self
  end

  def reverse
    @next ? @next.reverse.add(@val) : LList.new(@val)
  end

  def reverse!
    if @next
      swap = @next.val
      @next.val = @val
      @val = swap
      llist = @next      
      if llist.next
        @next = llist.next
        llist.next = nil
        self.reverse! << llist        
      end
    end
    self
  end

  def insert_sort
    sorted = LList.new @val
    to_sort = @next
    while to_sort
      #puts "%s   |   %s" % [sorted,to_sort]
      sorted.insert_order to_sort.val
      to_sort = to_sort.next
    end
    sorted
  end

  def min
    if @next.nil?
      @val
    else
      [@val,@next.min].min
    end
  end

end

class Array
  def to_list
    self.inject(LList.new) { |llist,x| llist.add x }.next
  end
end


if false

  n = 10
  a = Array.new(n) { rand 256 }
  b = a.to_list
  c = b.insert_sort

  if true

    puts  a.join(" "), b

    c = [1,2,3].to_list
    d = [7,8,9].to_list

    puts c << d

    c.next.next | [4,5,6].to_list

    puts c

    x = c.pop

    puts x, c

    puts c.insert_order 4
    puts c.insert_order 7.5

  puts "","reverse in new list"
    puts c.reverse
    puts c

  puts "","small test"
    h = [3,4,5,6].to_list
    puts h
    puts h.reverse!
    puts h

  puts "","reverse in place"
    puts c
    puts c.reverse!
    puts c

  end

  if false

    x = [5,7,8].to_list
    puts x
    x.insert_order 6
    puts x
    x.insert_order 3
    puts x

    puts b,c,d

  end

end
