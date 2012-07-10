#!/usr/local/bin/ruby


class String
  def score
    offset = 'a'.bytes.first - 1
    self.downcase.bytes.inject(0){|s,b| s += b-offset}
  end
end

File.open("prob42.dat",'r') do |file|

  scores = file.readlines[0].split(',').map{|s| s[1..-2].score }

  m = scores.max
  
  triangles = [].tap{ |a|
    i = 1
    while i*(i+1)/2 < m
      a << i*(i+1)/2
      i += 1
    end
  }

  puts scores.select{ |s| triangles.include? s}.length

end
