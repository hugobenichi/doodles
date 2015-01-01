#!/usr/bin/env ruby

require 'date'

# TODO: gerer temps de travail negatif
# TODO: ajouter et sauvegarder commentaire
# TODO: comment a little
module WorkTime

    module Default  # load configuration files or create one if not present
        class << self

            def feed                            # repeat daily hours
                from, to = self.start, self.end
                Fiber.new{ Fiber.yield [from, to] while true }
            end

            unless File.file? ENV["HOME"] + "/.worktimerc"
                File.open(ENV["HOME"] + "/.worktimerc", 'w') do |file|
                    [
                        [:start,    8.5],       # default of default
                        [:end,      18.25],
                        [:legal,    7.75],
                        [:pause,    1.0],
                    ].each do |pair| file.puts pair.join(",") end
                end
            end

            File.open(ENV["HOME"] + "/.worktimerc", 'r')    # load conf file
                .readlines.map{|s| s.chomp}
                .reject(&:empty?)
                .map{ |s| s.split(',') }.each{ |meth, string_val|   # parse the file
                    value = eval string_val                         # produce R values
                    define_method(meth.to_sym) do return value end  # map to methods
                }

        end
    end

    class Table                                 #represents a month worktime table
        attr_reader :year, :month, :days

        # TODO: add month parameter to adjust the table structure
        def initialize(feed = Default.feed, date = Date.today)
            @year, @month, @days = date.year, date.month, []
            x = Date.new(date.year, date.month, 1)
            while x.month == date.month
                @days <<  ( (x.sunday? || x.saturday?) ? nil : feed.resume )
                x = x.next
            end
        end

        def to_s        # formating TODO: move to "View" module for modular formatting
            @days.each_with_index.map{ |day, day_num| 
                prefix = ("%i/%02i/%02i" % [@year, @month, day_num+1]) + "        "
                if day.nil?
                    ""
                else
                    prefix + 
                      day.map{|h| WorkTime::hour_to_string h }.join(" - ") +
                      "    " + WorkTime.hour_to_string( WorkTime.overwork(*day))
                end
            }.join("\n") + "\ntotal: " + WorkTime.hour_to_string(self.overwork)
        end

        def total(pause=Default.pause)  # total time worked this month
            overwork(0, break_time)     # which methods is called ? instance or class ??
        end

        def overwork                    # overtime work  TODO: what about parameters ??
            @days.select{|day| day}.inject(0){|s, day| s += WorkTime.overwork(*day) }
        end

        def path                        # default path for saving work tables
            WorkTime::Default.dir + "%i%02i" % [@year, @month]
        end

        def save where=self.path        # write the table to file
            File.open(where,"w") do |f| f.puts self.to_s end 
            self
        end

        class << self
            # TODO: move this also to a "VIEW" module to put together parsing and unparsing
            def load path               # parse a file and
                month = 0
                hour_pattern = /\d+\/(\d+)\/\d+\s+(\d+:\d+)\s+-\s+(\d+:\d+)\s+(\d+:\d+)/
                this_month_hours = File.open(path,'r')
                                    .readlines
                                    .map{|s| s.match hour_pattern }
                                    .reject(&:nil?)
                                    .tap{|matches| month = matches[0][1].to_i }
                                    .map{|m| m[2..3].map{|s| WorkTime.string_to_hour s} }
                feed = Fiber.new{ this_month_hours.each{ |hours| Fiber.yield hours } }
                puts "this month is #{month}"
                if Date.today.month != month
                    $stderr.puts "current table not updated to this month" #TODO: better msg
                    #Kernel.exit(0)
                end
                Table.new feed # TODO: adjust month as parameter
            end
        end
    end

    class << self   # utility methods
        def hour_to_string hour
            base = hour.to_i
            min  = ((hour - base)*60) .to_i
            "%02i:%02i" % [base, min]
        end
        def string_to_hour string
            hour, min = *string.split(':').map(&:to_i)
            hour + Float(min) / 60
        end
        def overwork(from, to, daily_legal=Default.legal, pause=Default.pause)
            to - from - pause - daily_legal
        end
    end

end


date = Date.today
name = "%i%02i" % [date.year, date.month]
path = WorkTime::Default.dir + name

#unless File.file? file_name

if ARGV[0] && ARGV[0] == "new"
    unless File.file? path
        WorkTime::Table.new.save
    end
    Kernel.exit(0)
end

if ARGV[0] && ARGV[0] == "edit"
    Kernel.exec "gedit #{path}"
    Kernel.exit(0)
end

if ARGV[0] && ARGV[0] == "update"
    p WorkTime::Table.load(path).save
    Kernel.exit(0)
end

p WorkTime::Table.load(path)
#Kernel.exec "gedit #{path}"

#Kernel.exit(0)

#if ARGV[0]
#    p WorkTime::Table.load ARGV[0]
#else
#    p WorkTime::Table.new
#end


# think about interface
# add comments in work_time table
#wt init             create new work sheet for this month, using default
#wt current          print current worktime table
#                    puts error if current is not this month
#                        -> I need to parse date from table too !
#                         -> if month does not match this month exit or say you should init
#wt today from to    update current worktime table with from and to
#                    recalculate this day overtime and this month overtime
#                    save worktime table
#wt load             load some table (last month / 2 monthes ago)
#wt help

