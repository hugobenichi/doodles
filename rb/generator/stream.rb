

#state less immutable stream
class Stream
	def initialize( initial, eval, iterator, terminator = nil)
		@state, @eval, @next, @term = initial, eval, iterator, terminator
	end
	def head
		@eval.call(@state)
	end
	def tail
		new_state = @next.call(@state)
		if @term && @term.call(new_state)
	  	nil
	  else
	  	Stream.new( new_state, @eval, @next, @term )
		end
	end
	"""
	bug here: it takes one too much item
	"""
	def take(n)
		[self.head] + ((n == 1) ? [] :  self.tail.take(n-1))
	end
	def map &transform
		Stream.new( @state, Proc.new{ |from| transform.call( @eval.call(from) ) }, @next )
	end
	def filter &condition
		filtered_next  = Proc.new{ |new_state|
			new_state = @next.call(new_state)
			new_state = @next.call(new_state) until condition.call( new_state )  
			new_state
		}
		Stream.new( filtered_next.call(@state), @eval, filtered_next)
	end
end

     
""" 
#statefull stream
module StreamGenerator
	attr_accessor :state
	def next
		@state = 			
	end
	def take
	
	end
	def	map &transform
		alias_method :before_eval, :eval

		
	end
	def filter
	
	end
end

IntegerStream = Struct(:val) do
	include StreamGenerator
  def eval
		@val
	end
	def iter
		@val 
	end
end
"""
