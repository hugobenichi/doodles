#!/usr/local/bin/ruby

data = {}

data[:ind] = (0..15).to_a


data[:par] = data[:ind].map{ |i| (i-1)/2  }

data[:lc] = data[:ind].map{ |i| 2*i+1  }

data[:rc] = data[:ind].map{ |i| 2*i+2  }


data[:depth] = data[:ind].map{ |i| Math::log2(i+1).to_i  }


data[:relind] = data[:ind].map{ |i| i +1 - 2 ** data[:depth][i] }

data[:relpar] = data[:relind].map{ |i| i/2  }

data[:rellc] = data[:relind].map{ |i| 2*i  }

data[:relrc] = data[:relind].map{ |i| 2*i+1  }


data.each_key{ |k| print k.to_s, " " }
puts ""


data[:ind].each do |i|
  data.each_key{ |k| print data[k][i], "   " }
  puts ""
end


