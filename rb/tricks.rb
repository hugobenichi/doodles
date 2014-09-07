

rand 10..20

require "awesome_print"; ap [1,2,3]


#test substring
"abcdef"["cd"] # => "cd"
"abcdef"["fg"] # => nil

#string incrustation shortcut
@string = "etron"
puts "espece de gros #@etron"

#check syntax with -c  
# $ ruby -c awesome.rb 
# Syntax Ok

#syntax analysis
require "ripper"
require "awesome_print"

ap Ripper.sexp "puts {}.class"


# chained if
puts "tos" if 2 == 2 if 1 == 1

#zip
names = %w{ alice bob john }
ages = [25, 56, 35]
city = %w{ paris berlin tokyo}

zipped = names.zip ages, city
map = Hash[zipped]   # keys of the hash are the first element of the sub-arrays

names.zip( age, city) do |e| puts e end

#array of 10 to 20
[*10..20]

#json
require "json"

#meta prog access to current method and callee
__method__
__callee__

#binary representation
0b00010110

#hex representation
0xAAA3

#proc object source location
a = ->{ }
p a.source_location 
fail "assert failed in #{->{}.source_location}"  unless test   # get error place
method(:symb).source_location # get some method definition place / or before monkey patching

#prepend without creation
"world".prepend "hello "


# regular expression for variable creation

/(?<a>\w+) (?<b>\w+)/ =~ "hugo benichi, phd"
p a, b

#quick true and false
p !(), !!()


# __END__ and rewind

DATA.rewind # if no rewind, start at end of __END__
puts DATA.read

__END__

puts "do we get there ?"




