#!/usr/local/bin/ruby

tot = 2  # 1 x L2 + 2 x L1 

coins = [ 002, 05, 10, 20, 50]
multm = [ 100, 40, 20, 10, 04]


(0..multm[0]).each do |p2|
  p2_tot =  p2 * 2
  
  (0..multm[1]).each do |p5|
    p5_tot =  p5 * 5
    break if p5_tot > 200
    
    (0..multm[2]).each do |p10|
      p10_tot = p5_tot + p10 * 10
      break if p10_tot > 200
    
      (0..multm[3]).each do |p20|
        p20_tot = p10_tot + p20 * 20     
        break if p20_tot > 200
    
        (0..multm[4]).each do |p50|
          p50_tot = p20_tot + p50 * 50
          
          break if p50_tot > 200
          tot += 1
      
          break if p50_tot + 100 > 200         
          tot += 1
        
        end
      end
    end 
  end
end  

puts tot

if false
def look targ, sum, tokens
  case sum <=> targ
    when  0 then 1
    when -1 then tokens.inject(0){ |s,x| look( targ, sum+x+0, tokens) } +
                 tokens.inject(0){ |s,x| look( targ, sum+x+1, tokens) } + #remove the 1 coins only+
                 tokens.inject(0){ |s,x| look( targ, sum+x+2, tokens) }   #remove the 2 coins only solution
    when +1 then 0
  end
end
 
puts 1 +                              # remove the 1 x L2 coin from the search
     1 +                              # remove the 2 x L1 coins 
     look(100, 0, [50,20,10,5]) + # L1 + coins to 100
     1 +                              # 4 x 50p coins
     look(50,  0, [20,10,5] ) +   # 3 x 50p + 50
     look(100, 0, [20,10,5] ) +   # 2 x 50p + 100
     look(150, 0, [20,10,5] ) +   # 1 x 50p + 150
     look(200, 0, [20,10,5] )
end

if false
def look targ, sum, tokens
  case sum <=> targ
    when  0 then 1
    when -1 then tokens.inject(0){ |s,x| look( targ, sum+x, tokens)  } 
    when +1 then 0
  end
end
 
puts 1 +                              # remove the 1 x L2 coin from the search
     1 +                              # remove the 2 x L1 coins 
     look(100, 0, [50,20,10,5,2,1]) + # L1 + coins to 100
     1 +                              # 4 x 50p coins
     look(50,  0, [20,10,5,2,1] ) +   # 3 x 50p + 50
     look(100, 0, [20,10,5,2,1] ) +   # 2 x 50p + 100
     look(150, 0, [20,10,5,2,1] ) +   # 1 x 50p + 150
     look(200, 0, [20,10,5,2,1] )
end          
