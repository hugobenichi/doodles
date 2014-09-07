#!/usr/local/bin/ruby
#############################################
#                                           #
#   Example of Fifo usage with asynchronous #
#   write to output file using native       #
#   memory block                            #
#                                           #
#############################################

require 'circular_fifo'

len = 256
rep  = 100000
depth = 256
mem_siz = 4*len
name = "foo.data"
data = Array.new(len) { 0xfffffff }

start = Time.new

CircularFifo::MemoryFifo.async_write(depth, mem_siz, name, 'w') do |fifo|
    rep.times do fifo.push { |memory| memory.write_array_of_int data } end
    puts "finished pushing data"
end

puts Time.new - start
