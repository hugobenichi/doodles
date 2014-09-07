#!/usr/local/bin/ruby


class Future
    class << self
        attr_accessor :counter
    end
    @counter = 0
    def initialize &block
        @future, @value = block, self
    end
    def get
        #@value = @future.call if @value == self
        if @value == self
            c = Future.counter += 1
            puts "      Future#get >> init value #{c}"
            @value = @future.call
            puts "      Future#get >> #{c} -> #{@value}"
        end
        @value
    end
end


class VirtualEnum
    include Enumerable
    def initialize enum
        @traversals = []
        @enum = enum
        @eoi = Object.new
    end
    def each &block
        # create the fiber and returns it
        eoi = @eoi
        Fiber.new{ |item|
            puts "        VirtualEnum#each >> starting virtual traversal with Fiber"
            while item != eoi
                block.call item
                item = Fiber.yield
            end
        }.tap{|f| @traversals << f}
    end
    def each &block
        eoi = @eoi
        Fiber.new{
       #     puts "        VirtualEnum#each >> starting virtual traversal with Fiber"
            loop do
                item = Fiber.yield
                break if item == eoi
                block.call item
            end
        }.tap{|f| @traversals << f}.resume
    end
    def dispatch_all
        puts "        VirtualEnum#each >> starting actual traversal"
        @enum.each{ |item| @traversals.each{ |fiber| fiber.resume item } }
        @traversals.each{ |fiber| fiber.resume @eoi }
    end
end


class Synchronize
    def initialize
        @computations = []
    end
    def delay &block
        future = Future.new &block
        @computations << future
        future
    end
    def trigger &callback
        puts "running trigger in #{self}"
        unless @ran
            puts "running for first time"
            @ran = true
            @computations.each{ |b| b.get }
            @computations = []
            puts "calling callback"
            callback.call
        end
    end
    class << self
        def test
            s = Synchronize.new
            a = s.delay{ puts "foo"; "foo" }
            b = s.delay{ puts "bar"; "bar" }
            puts [a.get, b.get].join " "
        end
    end
end


class AccumulatorEnum          # implements usual traversal loops
    def each &block
        self.delay{ |virtual| puts "starting inner block of delay_each"; virtual.each &block }
    end
    def map &block
        self.delay{ |virtual| puts "starting inner block of delay_map"; virtual.map &block }
    end
    # TODO: debug inject which does not work (although map returns the good value ??)
    def inject(init, &block)
        self.delay{ |virtual| puts "starting inner block of delay_inject"; virtual.inject(init, &block) }
    end
end


#implem 1
class AccumulatorEnum
    class << self
        attr_accessor :counter
    end
    @counter = 0
    def initialize enum
        @virtual = VirtualEnum.new enum
        @computations = []
    end
    def delay &block                            # store a computation
        c = AccumulatorEnum.counter += 1
        me = self
        future = Future.new{ puts "    Accum#delay >> inner future #{c}"
                             block.call(@virtual) }
        @computations << future                 # store for later execution
        Future.new{ puts "  Accum#delay >> outer future #{c}"
                    me.run!; future.get }       # try to run all blocks and returns value
    end
    def run!                                    # internal method which
        unless @ran                             # triggers all stored computation
            @ran = true
            puts "Accum#run! >> running all inner futures"
            @computations.each{ |future| future.get }
            puts "Accum#run! >>  dispatching to all each blocks"
            @virtual.dispatch_all
        end
    end
end


accum = AccumulatorEnum.new  Array.new(5){|i| i}

r1 = accum.each{ |x| puts "in each: #{x}" }
r2 = accum.map{ |x| puts "in map: #{x}"; 2*x }
r3 = accum.inject(0){ |s,x| puts "in inject: #{s} #{x}"; s + x }

puts "check r2"
puts r2.get.class.to_s + " [" + r2.get.join(" ") + "]"
puts "check r3"
puts r3.get






#implem 2
#class AccumulatorEnum
#    def initialize enum
#        @virtual = VirtualEnum.new enum
#        @computations = []
#    end
#    def delay &block                            # store a computation
#        me = self
#        future = Future.new{ me.run!; block.call(@virtual) }
#        @computations << future                 # store for later execution
#        future
#    end
#    def run!                                    # internal method which
#        unless @ran                             # triggers all stored computation
#            @ran = true
#            @computations.each{ |future| future.get }
#            @virtual.dispatch_all
#        end
#    end
#end

#implem 3
#class AccumulatorEnum
#    def initialize enum
#        @virtual = VirtualEnum.new enum
#        @synch = Synchronize.new
#    end
#    def delay &block                            # store a computation
#        me, synch = self, @synch
#        @synch.delay{ 
#            synch.trigger{ @virtual.dispatch_all };
#            block.call(@virtual)
#        }
#    end
#end

