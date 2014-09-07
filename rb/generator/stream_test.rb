#!/usr/bin/env ruby


require "stream"

class Counter
	attr_reader :n
	def initialize n; @n = n; end
	def up; @n += 1; end
end
	
Total_filter = Counter.new(0)
Total_map    = Counter.new(0)

class Stream
	def chain val
		self.filter{|x| 
			#Total_filter.up
			#puts "filtering %i by %i mod %i" % [x,x,val]
			x % val == 0 #== (x%val)
		}.map{|y| 
		  #Total_map.up
			#puts "mapping %i to %i/%i" % [y,y,val]
			y/val
		}
	end
end


eval = Proc.new {|x| x }

iter = Proc.new {|x| x+1 }

integers = Stream.new( 1, eval, iter)


#t = integers.chain(3).chain(7).chain(6) #.chain(15).chain(33)
#puts t.head, t.tail.head, t.tail.tail.head 

long_enum = integers.chain(3).chain(7).chain(5).chain(2).chain(11) #.chain(15).chain(33)


#puts Total_filter.n, Total_map.n

t = Time.new

long_enum.take(20).each{ |x| puts x }

#20.times{ 
#  puts long_enum.head
#  long_enum = long_enum.tail 
#}

puts Time.new - t

"""
puts integers.filter{ |x| 0 == (x % 2) }.tail.tail.head

puts integers.take(10).join(" "), 
     integers.filter{|x| 0 == (x%2)}.take(10).join(" "),
     integers.map{|x| x*2}.take(10).join(" ")
"""
