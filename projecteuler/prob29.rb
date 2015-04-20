#!/usr/local/bin/ruby

tot, sta, stp = 0,  2, (ARGV[0] || 100).to_i

mark = Array.new(stp+1){ Array.new(stp+1){nil} }

(sta..stp).each do |a|
  (sta..stp).each do |b|
    i = b
    m = a
    while i < stp+1 && m < stp+1    
      in_pow = i < stp+1 && mark[a][i].nil?
      in_bas = m < stp+1 && mark[m][b].nil?      
      tot       += 1 if in_pow || in_bas 
      mark[a][i] = 1 if in_pow
      mark[m][b] = 1 if in_bas        
      i += b
      m *= a    
    end    
  end
end

puts mark.inject(0){ |s,col| s += col.select{|x| x == 1}.length }


if false
(sta..stp).each do |a|

  (sta..stp).each do |b|
  
    i = b
    m = a
    while i < stp+1
      
      if i < stp+1 && mark[a][i].nil?
        tot += 1
        mark[a][i] = 1
        mark[m][b] = 1 if m < stp+1 && mark[m][b].nil?
      end
      if m < stp+1 && mark[m][b].nil?
        tot += 1
        mark[m][b] = 1
        mark[a][i] = 1 if i < stp+1 && mark[a][i].nil?
      end
      
      i += b
      m *= a
      
    end
    
  end
  
end
end

puts tot
