

require "./ruby/analysis"  
include Analysis


param = {
  grid_max:         6.4,        
  grid_pts:         32,        
  grid_typ:         :xy,    
  #grid_typ:         :rad,
  algorithm:        :kirt,
  #algorithm:        :sirt,
  #algorithm:        :maxlik,
  cutoff:           7.0,
  photon:           15,
  iteration:        50,
  renorm:           true,
}


margi   = Quix::Marginal.from_file ARGV[0]
grid    = Quix::G.new( param[:grid_typ], param[:grid_max], param[:grid_pts], param[:grid_pts])
wigner  = case param[:algorithm]
  when :irt     then margi.tomography( :kirt, grid, param[:cutoff] )   
  when :kirt    then margi.tomography( :kirt, grid, param[:cutoff] )
  when :sirt    then margi.tomography( :sirt, grid )
  when :maxlik  
    margi.tomography( 
      :maxlik, 
      {iterations: param[:iteration], photons: param[:photon], report: 10} 
    ).wigner(grid)
end

wigner.plot.draw
puts "","purity: #{wigner.purity}", "norm: #{wigner.norm}", "neg: #{wigner.neg}", ""

[[:kirt, param[:cutoff]],[:sirt]].each do |alg| 
  puts "W(0,0) = %f +- %f   (#{alg[0]})\n" % margi.negvar(*alg)
end

STDIN.gets

puts "printing wigner function ... "
wigner.plot.path = ARGV[0].gsub(".dat","") + "_wigner_%s_%i" % [param[:grid_typ].to_s, param[:grid_pts]]
wigner.plot.save_data
wigner.plot.save_png


