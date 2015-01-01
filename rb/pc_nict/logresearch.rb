#!/usr/local/bin/ruby

require 'date'

first = today = Date.today
first = first.prev_day until first.day == 1


dir    = File.open(ENV["HOME"] + "/.logresearchrc", 'r').readlines.map(&:chomp).join ""
name   = "log %02i-%02i"  % [today.year % 100, today.month]

offset  = 0
column  = 0

unless File.file? dir + name
    File.open(dir + name, 'w') do |f|
        m = first.month
        while m == first.month
            if first.saturday?
                first = first.next_day
                next
            elsif first.sunday?
                f.puts ""
            else
                f.puts "%02i/%02i" % [first.month, first.day]
            end
            first = first.next_day
        end
    end
else
    puts "log file already exist for this month"
    tag = "%02i/%02i" %  if today.friday?
        [today.month, today.day+3]
    else
        [today.month, today.day+1]
    end
    File.open(dir + name, 'r').readlines.each.with_index { |line, num|
        offset = num
        break if line.include? tag 
        column = line.length
    }
end


Kernel.exec 'gedit +%i:%i "%s" &' % [offset, column, (dir + name)] #.gsub(" ", '\ '))]


if false
counter = 0
file = [].tap{ |lines| 
    m = first.month
    while m == first.month
        if first.saturday?
            first = first.next_day
            next
#        elsif first.sunday?
#            lines << ""
#            counter += 1
        else
            lines << (first.sunday? ? "" : "%02i/%02i" % [first.month, first.day])
            offset = counter if first.day == today.day
            counter += 1
        end
        first = first.next_day
    end
}
end
