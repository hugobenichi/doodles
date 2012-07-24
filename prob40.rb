#!/usr/local/bin/ruby

"".tap{ |decimals|
  200000.times{ |i| decimals << i.to_s } # 200000 generates a big enough string
  puts [1e0,1e1,1e2,1e3,1e4,1e5,1e6].map{|i| decimals[i].to_i }.inject(1){|p,x| p *= x }
}


