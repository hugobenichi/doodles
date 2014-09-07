

require "./ruby/analysis"  
include Analysis


data = Reader::open_multi_data ARGV
write_data data.variance(30), data.path + "/variance.dat"


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
  #algorithm:        :maxlik,
  #algorithm:        :sirt,
  #algorithm:        :kirt,
  cutoff:           7.0,
  photon:           15,
  iteration:        50,
  renorm:           true,
  center:           250,
  center_ax:        196,
  center_ap:        196,
  average:          30,
  gamma:            0.035,
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


  if false

    params[:algorithm] = :sirt

    choose_parameters(params) do |x| puts "cutoff %f : %f" % x end

    margi, wigner = *process_tomography(params)

    wigner.plot.draw
    puts "","purity: #{wigner.purity}", "norm: #{wigner.norm}", "neg: #{wigner.neg}", ""

    puts "printing marginal distribution ... "
    margi.plot.path = params[:path] + "/marginal"
    margi.plot.set.xrange "[0:pi]"
    margi.plot.save_data
    margi.plot.save_png

    STDIN.gets

    puts "printing wigner function ... "
    wigner.plot.path = params[:path] + "/wigner_%s_%i" % [params[:grid_typ].to_s, params[:grid_pts]]
    wigner.plot.save_data
    wigner.plot.save_png

  #end
  else
  params[:algorithm] = :conditional

  #if true

    prepare_path params        
    multi = data.multi_to_marginal(params)
    multi.plot.path = params[:path] + "/ap#{params[:center_ax]}_ax#{params[:center_ap]}"
    condneg = multi.to_condneg( {style: :fraction} )

    prob = condneg.prob
    prob.plot.path = params[:path] + "/probability_ap#{params[:center_ax]}_ax#{params[:center_ap]}"
    prob.plot.save_data
    prob.plot.save_png

    condneg.plot.save_data
    condneg.plot.save_png

  end


  if false

    prepare_path params

    params[:center_ax] -= 2

    3.times do

      params[:center_ap] = 194

      3.times do

        multi = data.multi_to_marginal(params)
        multi.plot.path = params[:path] + "/ap#{params[:center_ax]}_ax#{params[:center_ap]}"
        #multi.plot.save_data
        condneg = multi.to_condneg( {style: :fraction} )
        condneg.plot.save_data
        condneg.plot.save_png

        params[:center_ap] += 2

      end

      params[:center_ax] += 2

    end

  end
