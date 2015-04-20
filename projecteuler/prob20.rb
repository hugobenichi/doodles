#!/usr/local/bin/ruby

n = 100

digits = [1]

(1..n).each do |i|
  ret = 0
  digits.each_index do |j|   
    rez = digits[j] * i + ret
    ret = rez / 10
#puts "%i x %i = %i:   digit %i | ret %i" % [digits[j], i, rez, rez % 10, ret]
    digits[j] = rez % 10  
  end
  while ret != 0
    digits << ret % 10
    ret /= 10
  end
#puts "%i! = %s" % [i, digits.reverse.join]
#uts ""
end


puts digits.reverse.join, "", digits.inject(0){|s,c| s += c}
