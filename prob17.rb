#!/usr/local/bin/ruby


max = 1000

length = Array.new(100){nil}

# 1 to 9
length[1] = "one"
length[2] = "two"
length[3] = "three"
length[4] = "four"
length[5] = "five"
length[6] = "six"
length[7] = "seven"
length[8] = "eight"
length[9] = "nine"

# 10 to 19
length[10] = "ten"
length[11] = "eleven"
length[12] = "twelve"
length[13] = "thirteen"
length[14] = "fourteen"
length[15] = "fifteen"
length[16] = "sixteen"
length[17] = "seventeen"
length[18] = "eighteen"
length[19] = "nineteen"

# dizens
length[20] = "twenty"
length[30] = "thirty"
length[40] = "forty"
length[50] = "fifty"
length[60] = "sixty"
length[70] = "seventy"
length[80] = "eighty"
length[90] = "ninety"

hundred = "hundred".length

length.map!{ |e| e ? e.length : 0 }

(2..9).each{ |i| (1..9).each{ |j| length[10*i+j] = length[10*i] + length[j] } }

(1..9).each{ |i|
  length[100*i] = hundred + length[i] #one hundred, two hundred, ...
  (1..99).each{ |j| length[100*i + j] = length[100*i] + 3 + length[j] }
} 

puts length.inject(0){|s,c| s += c} + "onethousand".length
