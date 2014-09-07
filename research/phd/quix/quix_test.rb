require "./quix"

m, x, y = 6.0, 32, 32
g1 = Q::G.xy m, x, y
g2 = Q::G.rad m, x, y

#puts g1.x[4], g1.x[2], g1.y[8],  g1[4][12]

gene = Q::Generator.vacuum
gene = Q::Generator.coherent 2.0, 0.0
gene = Q::Generator.one
#gene = Q::Generator.vacuum
#gene = Q::Generator.vacuum


m1 = Q::Generator.one_tele(0.8).marginal(500000)
m2 = Q::Generator.one.marginal(500000).tele(0.8)


w1 = m1.tomography(:sirt, g2)
w2 = m2.tomography(:sirt, g2)

puts w1.dist_over(w2), w1.dist_l2(w2)

w1.plot.draw
w2.plot.draw


#w = gene.wigner g2
#puts w.norm
#w.plot.draw
#w.photons(10).plot.draw



#m = gene.marginal 300000
#m.plot.draw

#gene = Quix::Generator.one

#[3.0, 5.0, 7.0, 9.0].map{|x|x/8.68}.each do |x|
#  m = gene.marginal(300000).tele(x)
#  puts m.negativity(:kirt), m.negativity(:sirt)
#end


if false

  r, l = 1.0, 1.0/Math::sqrt(2.0)
  a, s = l*Math::exp(+r), l*Math::exp(-r)
  c    = Math::sqrt Math::cosh(+2*r)

  m1 = Quix::Generator.vacuum.marginal 20000
  m2 = Quix::Generator.vacuum.marginal 20000
  m3 = Quix::Generator.vacuum.marginal 20000

  ( m1.*(a) ).+( m2.*(s) ).plot.draw
  ( m1.*(a) ).-( m2.*(s) ).plot.draw
  ( m3.*(c) ).plot.draw

  STDIN.gets
end

#10.times do puts gene.marginal(100000).negativity :kirt, 7.0 end


#puts m.negativity :kirt, 7.0

#w2 = m.tomography( :kirt, g2, 7.0, 4 )
#m.tomography( :sirt, g2, Q::Series.new(8, 40,10.0), 4 ).plot.draw
#w.plot.draw

#w2.photons(15).plot.draw

#h = gene.histogram g2.to_h!


#fock = Q::Fock.new g2, 16
#puts fock[1][1].re[0][0]


#d = gene.dmatrix 16
#d.plot.draw
#d.diag.plot.draw
#d.wigner(g2).plot.draw


if false
  m = gene.marginal 100000

  5.times do 

    s = Time.new
    #m.tomography(:maxlik, {n_thread: 0, iterations: 40, report: 1}).wigner(g1).plot.draw
    m.tomography(:maxlik, {n_thread: 4, iterations: 40, report: 1}).wigner(g1).plot.draw
    #m.tomography(:kirt, g2).plot.draw
    #m.tomography(:sirt, g2).plot.draw
    puts "elapsed: #{Time.new-s}"

  end
end

#puts "kernel", m.negvar(:kirt, 7.0), "","series",m.negvar(:sirt)


#sleep 1
STDIN.gets
#Quix.todo?







