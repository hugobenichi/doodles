

require "./ruby/analysis"  
include Analysis


if false
  n = 200
  4.times do
    data = Reader::open_data ARGV << "tomo=#{n}"
    write_data data.variance, data.path + "/variance#{n}.dat"
    n *= 10
  end
end


if true

data = Reader::open_data ARGV
write_data data.variance, data.path + "/variance#{200}.dat"


params = {
  data:             data,
  path:             data.path,
  phase_smoothing:  1, 
  phase:            :original,
  #phase:            :random,
  grid_max:         6.0,        
  grid_pts:         32,        
  grid_typ:         :xy,   
  #grid_typ:         :rad,
  algorithm:        :irt,
  #algorithm:        :sirt,
  #algorithm:        :maxlik,
  cutoff:           7.0,
  photon:           15,
  iteration:        50,
  renorm:           true,
  center:           1108,
  average:          30,
  gamma:            0.044,
  kappa:            0.000,
  alpha:            0.0,
  projector:        4,
  unshift:          true,
  p_distrib:        true,
  variance:         true,
}

 
puts "integrated value before removing dc component %f" % data.mean.inject(0.0){|x,y| y += x}
data.remove_meandc
puts "after removing %f" % data.mean.inject(0.0){|x,y| y += x}


#choose_parameters params  do |x| puts "cutoff %f : %f" % x end
#choose_algorithm  params
#write_data( data.variance(), data.path + "/variance_smooth.dat") if params[:variance]


margi, wigner, photons = *process_tomography(params)
wigner.plot.draw
puts "","purity: #{wigner.purity}", "norm: #{wigner.norm}", "neg: #{wigner.neg}", ""


params[:grid_typ] = :rad

margi, wigner, photons = *process_tomography(params)
wigner.plot.draw
puts "","purity: #{wigner.purity}", "norm: #{wigner.norm}", "neg: #{wigner.neg}", ""


params[:algorithm] = :sirt
margi, wigner, photons = *process_tomography(params)
wigner.plot.draw
puts "","purity: #{wigner.purity}", "norm: #{wigner.norm}", "neg: #{wigner.neg}", ""


#[[:kirt, params[:cutoff]],[:sirt]].each do |alg| 
#  puts "W(0,0) = %f +- %f   (#{alg[0]})\n" % margi.negvar(*alg)
#end

#puts "printing marginal distribution ... "
#margi.plot.path = params[:path] + "/marginal"
#margi.plot.set.xrange "[0:pi]"
#margi.plot.save_data
#margi.plot.save_png


#puts "printing photon distribution ... "
#photons.plot.path = params[:path] + "/photon_distrib"
#photons.plot.save_data
#photons.plot.save_png


STDIN.gets

#puts "printing wigner function ... "
#wigner.plot.path = params[:path] + "/wigner_%s_%i" % [params[:grid_typ].to_s, params[:grid_pts]]
#wigner.plot.save_data
#wigner.plot.save_png

end
