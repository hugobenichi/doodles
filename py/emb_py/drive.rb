#!/usr/local/bin/ruby


#subproc = 
Kernel.open("| ./echo.py", "rw") do |subproc|

%w{ tos foo bar etron}.each{ |s| 
    puts "sending %s from Ruby to subproc" % s
    subproc.puts s
    puts subproc.readline 
}
end
#subproc.close

