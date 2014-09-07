require "./quix"

m, x, y = 6.0, 32, 32
g1 = Q::G.xy m, x, y
g2 = Q::G.rad m, x, y


m1 = Q::Generator.vacuum.marginal 100000
m2 = Q::Generator.one.marginal 100000
m3 = Q::Generator.thermal(1.0).marginal 100000
#m4 = Q::Generator..marginal 50000

settings = {photons: 8, iterations: 30, report: 0}

s = Time.new
#5.times do
m1.tomography(:diag,settings).wigner(g1).plot.draw
m2.tomography(:diag,settings).wigner(g1).plot.draw
m3.tomography(:diag,settings).wigner(g1).plot.draw
#end
puts "","elpased",Time.new-s

STDIN.gets


