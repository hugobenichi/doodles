#!/usr/local/bin/ruby

tot, sta, stp = 0,  2, 100

mark = Array.new(stp+1){ Array.new(stp+1){nil} }


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

puts tot
