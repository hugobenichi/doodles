class Fixnum
  def palindrome?
    ori = self
    rev = 0
    while ori > 0
      rev  = 10*rev + ori % 10
      ori /= 10
    end
    rev == self
  end
end

n, m = 999, 999
rez = 0
while n > 99 && m > 99
  nm = n*m
  if nm > rez && (nm).palindrome? 
    rez = nm
  end
  if n < m
    m -= 1
  else
    n -= 1
    m = 999
    break if n*m < rez 
  end
end

puts  rez
