#!/usr/bin/env ruby

class Enumerator 
  def lselect(&condition) 
    Enumerator.new do |output| 
      self.each{ |val| output.yield(val) if condition.call(val) } 
    end 
  end 
 	def lmap(&transform)
 		Enumerator.new do |output|
 			loop do
 				output.yield transform.call( self.next )
 			end
 		end
 	end
 	def lmap(&transform)
 		Enumerator.new do |output|
 			self.each{ |input| output.yield transform.call input }
 		end
 	end 	
 	def chain value
		self.lselect{ |y| y % value == 0 }.lmap{ |x| x / value }
 	end
end

integers = Enumerator.new do |output|
	n = 1
	loop do
	  output.yield n
	  n += 1
	end
end


long_enum = integers.chain(3).chain(7).chain(6).chain(15).chain(33)


t = Time.new

20.times{ puts long_enum.next }

puts Time.new - t
