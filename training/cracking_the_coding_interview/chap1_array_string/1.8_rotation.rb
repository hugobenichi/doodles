#!/usr/bin/env ruby

s = (ARGV[0] || "erbottlewat" ).downcase
t = (ARGV[1] || "waterbottle" ).downcase

puts "is %s a rotation of %s ?\n=> %s" % [s,t, ( (t*2).include? s ).to_s   ]

