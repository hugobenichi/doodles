#!/usr/local/bin/ruby

require 'date'


from, to = Date.today, Date.today
from = from.prev_day until from.wednesday?
to   = to.next_day   until to.tuesday?


week_numbers = [from.month, from.day, to.month, to.day]


dir    = File.open(ENV["HOME"] + "/.reportrc", 'r').readlines.map(&:chomp).join ""
name   = "report_benichi_%02i%02i_to_%02i%02i.txt"  % week_numbers
header = "Week %02i/%02i to %02i/%02i"              % week_numbers

unless File.file? dir + name
    File.open(dir + name, 'w'){ |f| f.puts header, "" }
else
    puts "report file already exist for this month"
end

Kernel.exec "gedit +2 " + dir + name 
