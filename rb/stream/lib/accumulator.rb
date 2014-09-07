

# accumulates a number of futures and triggers them off at the same time
# when the first future is evaluated
class Accumulator
    def initialize
        @computations = []
    end
    def delay &block                            # store a computation
        me, future = self,  Future.new(&block)  # prepare block for later execution
        @computations << future                 # store for later execution
        Future.new{ me.run!; future.get }       # try to run all blocks and returns value
    end
    def run!                                    # internal method which
        unless @ran                             # triggers all stored computation
            @computations.each{ |future| future.get }
            @ran = true
        end
    end
end


puts "testing Accumulator"
l = Accumulator.new

a = l.delay do puts "running foo"; "foo" end
b = l.delay do puts "running bar"; "bar" end

puts a.get
puts a.get
puts b.get
