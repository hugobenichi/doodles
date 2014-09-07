#!/usr/local/bin/ruby

class VirtualEnum
    include Enumerable
    def initialize enum
        @traversals = []
        @enum = enum
        @eoi = Object.new
    end
    def each &block
        eoi = @eoi
        @traversals << Fiber.new do |item|
            puts "        VirtualEnum#each >> starting virtual traversal with Fiber"
            while item != eoi
                block.call item
                item = Fiber.yield
            end
        end
    end
    def dispatch_all
        puts "        VirtualEnum#each >> starting actual traversal"
        @enum.each{ |item| @traversals.each{ |fiber| fiber.resume item } }
        @traversals.each{ |fiber| fiber.resume @eoi }
    end
end

    accum = VirtualEnum.new  Array.new(5){|i| i}

    puts "preparing blocks"
    r1 = accum.each{ |x| puts "in each 1: #{x}" }
    r2 = accum.each{ |x| puts "in each 2: #{x}" }
    r3 = accum.each{ |x| puts "in each 3: #{x}" }

    puts "", r1.class, r2.class, r3.class
    
    puts "starting block"
    accum.dispatch_all
