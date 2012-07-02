#!/usr/local/bin/ruby



def filter sequence
  sequence.select{|x| x}.map.each_with_index{ |x,i|
    if sequence[i+3]
      sequence[i] * sequence[i+1] * sequence[i+2] * sequence[i+3]
    else
      0
    end
  }.max
end

def diag_right matrix, i, j
  [0].tap{ |out|
    while matrix[i] && matrix[i][j]
      out << matrix[i][j]
      i += 1
      j += 1
    end
  }
end

data = File.open('./prob11.dat','r').readlines.map{ |line| line.split(" ").map{ |s| s.to_i } }
puts [].tap{ |all|
  data.each{ |row| all << row }
  data.transpose.each{ |col| all << col }
  mirr = data.map{ |row| row.reverse}
  data.each_index{ |i| 
    all << diag_right( data, i, 0) 
    all << diag_right( mirr, i, 0) 
  }
  data[0].each_index{ |j|
    all << diag_right( data, 0, j) 
    all << diag_right( mirr, 0, j) 
  }
}.map{|seq| filter seq}.max
