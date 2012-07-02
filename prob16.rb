#!/usr/local/bin/ruby

def double old
  offset = '0'.bytes.first
  "".tap{ |new|   
    ret = 0;
    old.reverse.each_byte{ |c| 
      rez = 2 * (c-offset) + ret
      ret = rez/10
      new << "#{rez%10}"
    }
    new << "1" if ret != 0 
  }.reverse
end

def sum string
  offset = '0'.bytes.first
  string.each_byte.inject(0){ |s,c| s += (c-offset) }
end


string = "1"
1000.times{ 
  string = double string 
  #puts "%i:  %i" % [string, sum(string)]
}

puts sum(string)
