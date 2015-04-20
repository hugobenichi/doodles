#!/usr/local/bin/ruby

def score string
  offset = 'a'.bytes.first - 1
  string.bytes.inject(0){|s,b| s += b-offset }
end

#puts  File.open("./names.txt").readlines[0].split(',').sort.map.each_with_index
#        .map{ |s,i| "%s %i" % [s, (i+1)*score( s[1..-2].downcase )] } #.inject(0){|s,x| s+=x}

puts  File.open("./names.txt").readlines[0].split(',').sort.map.each_with_index
        .map{ |s,i| (i+1)*score( s[1..-2].downcase ) }.inject(0){|s,x| s+=x}
