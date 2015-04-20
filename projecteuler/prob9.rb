n = (ARGV[0] || "1000").to_i

(1..n).each do |a|
  (a..n).each do |b|
    if a + b + Math::sqrt(a*a + b*b) == n
      puts a*b*Math::sqrt(a*a + b*b)
    end
  end
end


