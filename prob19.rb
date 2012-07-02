#!/usr/local/bin/ruby


#
# bugged
#  gives 170, but the good answer is 171
#

day2month = Array.new(13){31} 
[4,6,9,11].each{ |i| day2month[i] = 30}
day2month[2] = 28 

day = 1

day_num = 1
month = 1
year = 1900

month_day = day2month[month]
 
tot = 0

while year < 2001

  if day == 7 && day_num == 1 && year > 1900
    tot += 1
    #puts "found Sunday on %i-%.2i-%.2i" % [year, month, day_num]
  end 

  if day == 7 
    day = 1
  else
    day += 1
  end
  
  if day_num == month_day
    day_num = 1    
    if month == 12
      year += 1
      month = 1
    else
      month += 1
    end
    month_day = day2month[month]
    if month == 2 && (year % 4 == 0) && (year % 400 != 0)   
      month_day += 1 
      puts "%i is a leap year" % year
    end
    puts "%i - %.2i: %i days" % [year, month, month_day]
  else
    day_num += 1
  end

end

puts tot
