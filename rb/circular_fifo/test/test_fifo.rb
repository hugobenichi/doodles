#!/usr/local/bin/ruby
#############################################
#                                           #
#   Example of Fifo usage with 2 threads    #
#                                           #
#   One thread = one writer                 #
#   Another thread = one reader             #
#                                           #
#############################################

require 'ffi'
require 'circular_fifo'


include CircularFifo

size = 256
num  = 1024 #65536

#f = Fifo.new(size) { FFI::MemoryPointer.new :int, 1 }
#f = CircularFifo::memory_buffer(size, 4)
f = CircularFifo::MemoryFifo.new size, 4

start = Time.new


writer = Thread.new do
    begin
        num.times { |i|
            f.push { |p| puts "writing @ #{i}"; p.write_int i }
            #f.push { |p| p.write_int i }
        }
    rescue Exception => orz
        puts orz
    end
end


reader = Thread.new do
    collect = []
    begin
        num.times { |i|
            #f.pop { |p| puts "reading @ #{i}: #{p.read_int}" }
            f.pop { |p| collect << p.read_int }
        }
    rescue Exception => lolz
        puts lolz
    end
    puts collect
            .each_with_index
            .map{|x,y| x == y} # check if index == value @ index
            .inject(true){|s,b| s && b}
end


writer.join
reader.join


puts Time.new - start
