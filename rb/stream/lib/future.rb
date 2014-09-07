#!/usr/local/bin/ruby

#implem 1
#class Future                                # save a block for later evaluation 
#    def initialize &block                   # init a Future value with saved block 
#        @future, @value = block, self
#    end
#    def get                                 # run the blocks if not yet ran
#        @value = @future.call if @value == self
#        @value                              # return the value
#    end
#end

#puts "testing Future"
#a = Future.new do puts "foo"; "foo" end
#b = Future.new do puts "bar"; "bar" end
#puts [ "future is ", a.get].join ""
#puts [ "future is ", a.get].join ""
#puts b.get


#implem 2
#class Proc
#    def future(*args)
#        me = val = self
#        Proc.new do 
#            val = me.call(*args) if val == me
#            val
#        end
#    end
#    def self.future( *args, &block)
#        block.future(*args)
#    end
#end
##a = Proc.new{ |*arg| puts "foo #{arg}"; "bar" }.future([0,1,2], "etron")
#a = Proc.future([0,1,2], "etron"){ |*arg| puts "foo #{arg}"; "bar" }
#puts "testing FProc"
#puts a[]
#puts a[]


#implem 3
#class Proc
#    def future(*args)
#        block = self
#        Fiber.new do
#            val = block.call(*args)
#            Fiber.yield val while true
#        end
#    end
#end

#a = Proc.new{ |*arg| puts "foo #{arg}"; "bar" }.future([0,1,2], "etron")
#puts "testing FProc"
#puts a.resume
#puts a.resume


#implem 3.1
#class Future < Fiber
#    def initialize &block
#        super do
#            val = block.call
#            Fiber.yield val while true
#        end
#    end
#    alias_method :get, :resume
#end


#a = Future.new do puts "foo"; "bar" end

#puts a.get
#puts a.get




