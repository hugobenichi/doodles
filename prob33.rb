#!/usr/local/bin/ruby

num   = 1
denum = 1
tot = 0

(1..9).each do |a|
  (0..9).each do |b|
    (0..9).each do |c|
    
      abc = ( a*10+b )*c
      
      # case d = a && ab / cd   
      if c != 0 && abc == (c*10+a)*b && ( a*10+b ) < ( c*10+a ) 
        puts "found: %i%i/%i%i" % [a,b,c,a]
        tot += 1    
        num *= ( a*10+b )
        denum *= ( c*10+a ) 
      end
      
      # case d = b && ab / cb
      if c != 0 && b != 0 && abc == (c*10+b)*a && ( a*10+b ) < ( c*10+b )
        puts "found: %i%i/%i%i" % [a,b,c,b]  
        tot += 1
        num *= ( a*10+b )
        denum *= ( c*10+b )      
      end
      
      # case d = a && ab / dc      
      if abc == (a*10+c)*b && ( a*10+b ) < ( a*10+c )
        puts "found: %i%i/%i%i" % [a,b,a,c]
        tot += 1
        num *= ( a*10+b )
        denum *= ( a*10+c )      
      end
      
      # case d = b && ab / bc 
      if b != 0 && abc == (b*10+c)*a && ( a*10+b ) < ( b*10+c )
        puts "found: %i%i/%i%i" % [a,b,b,c]
        tot += 1
        num *= ( a*10+b )
        denum *= ( b*10+c )     
      end      
        
      end
    end
end

puts "total: %i   prod:  %i / %i" % [tot, num, denum]
