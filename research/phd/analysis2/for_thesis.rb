

require "./ruby/analysis"  
include Analysis


data = Reader::open_multi_data ARGV


params = {
  data:             data,
  path:             data.path,
  phase_smoothing:  1, 
  phase:            :original,
  #phase:            :random,
  grid_max:         6.0,        
  grid_pts:         32,        
  grid_typ:         :rad,
  algorithm:        :conditional,
  cutoff:           7.0,
  photon:           15,
  iteration:        50,
  renorm:           true,
  center:           250,
  center_ax:        190,
  center_ap:        210,
  average:          30,
  gamma:            0.030,
  kappa:            0.100,
  alpha:            0.0,
  projector:        4,
  unshift:          true,
  p_distrib:        false,
}


  [data, data.ax, data.ap].each do |f|
    puts "", f.mean.inject(0.0){|x,y| y += x}
    f.remove_meandc
    puts f.mean.inject(0.0){|x,y| y += x}, "" 
  end

  prepare_path params   
     
  m100 = data.multi_to_marginal(params)
  m100.plot.path = params[:path] + "/margi100"
  m100.plot.save_data

  m10 = m100.select 0.1
  m10.plot.path = params[:path] + "/margi10"
  m10.plot.save_data 

  m1 = m10.select 0.1
  m1.plot.path = params[:path] + "/margi1"
  m1.plot.save_data 


