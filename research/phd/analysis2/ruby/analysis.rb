module Analysis

  DIR = File.expand_path(File.dirname(__FILE__)) unless defined? DIR

  require "ffi"
  require "fileutils"
  require "#{DIR}/../../quix/quix"
  require "#{DIR}/frames"
  require "#{DIR}/multiframes"
  require "#{DIR}/reader"
  require "#{DIR}/phase"
  require "#{DIR}/projector"

  extend FFI::Library
  ffi_lib "#{DIR}/../lib/" + (ENV.has_key?("OS") ? "analysis32.dll" : "libanalysis.so")

  [ 
    [:frames_alloc,             [:int,:int,:int],                           :pointer],
    [:frames_free,              [:pointer],                                 :void],
    [:frames_copy_to_shot,      [:pointer,:pointer,:int],                   :void],
    [:frames_copy_to_tomo,      [:pointer,:pointer,:int],                   :void],
    [:frames_apply_window,      [:pointer,:pointer,:pointer,:pointer],      :void],
    [:frames_apply_window_smth, [:pointer,:pointer,:pointer,:pointer,:int], :void],
    [:frames_mean,              [:pointer],                                 :pointer],
    [:frames_remove_dc,         [:pointer],                                 :void],
    [:frames_remove_meandc,     [:pointer],                                 :void],
    [:var_addto,                [:pointer,:pointer,:int],                   :void],
    [:var_addto_smth,           [:pointer,:pointer,:int,:int],              :void],
    [:mode_apply,               [:pointer,:pointer,:int],                   :double],
    [:var_comp,                 [:pointer,:int],                            :double],
    [:norm,                     [:pointer,:double,:int],                    :void],
    [:phase_acos,               [:pointer,:int],                            :void],
  ].each do |sig| attach_function *sig end

  def write_data data, path
    File.open(path,"w") do |file| data.each {|d| file.puts d} end
    data
  end

  def write_multidata multidata, path
    puts "writing multidata to #{path} ... "
    File.open(path,"w") do |file| 
      multidata[0].each_index do |i|
        multidata.each do |data|
          file.print data[i]
          file.print " "
        end
        file.print "\n"
      end 
    end
    multidata
  end

  def scan_parameters( parameters)
    order = [:center,:average,:gamma,:kappa,:alpha]
    while true
      print "\nenter origin, average, gamma, kappa ... "
      begin
        new = STDIN.gets.split(" ").map!{|s| 
          x = s.to_f
          x.to_i == 0 ? x : x.to_i
        }
        new.each_index{ |i| parameters[order[i]] = new[i] }
        yield(parameters) if block_given?
      rescue
        puts "please enter correct values"
      end
      puts "continue?"
      break if STDIN.gets.chomp.upcase[0] == "Y"
    end
  end

  def choose_parameters( parameters )
    scan_parameters( parameters) do |new_param|
      new_param[:data].negativity(new_param).each{ |x| yield(x) if block_given? }
    end
  end

  def choose_algorithm parameters
    puts "","tomography algorithm","1: irt   2: maxlike "
    if STDIN.gets.chomp.to_i == 2
      puts  "using maxlike", "enter number of iteration "
      algo, iter = :maxlik, STDIN.gets.chomp.to_i
      parameters[:algorithm] = algo
      parameters[:iteration] = iter
    else
      puts  "using irt", "enter cutoff "
      algo, cutoff = :irt, STDIN.gets.chomp.to_i
      parameters[:algorithm] = algo
      parameters[:cutoff] = cutoff
    end
  end

  def prepare_path param
    FileUtils::makedirs param[:path] += case param[:algorithm] 
      when :maxlik
        "/maxlik_N#{param[:photon]}_iter#{param[:iteration]}"
      when :sirt
        "/sirt_M8_N40_L10"
      when :kirt
        "/irt_cutoff#{param[:cutoff]}"
      when :irt
        "/irt_cutoff#{param[:cutoff]}"
      when :conditional
        "/conditional"
    end + "_center%i_avrg_%i_gamma%f_kappa%f" % [:center,:average,:gamma,:kappa].map{ |k| 
      param[k]
    }
  end

  def process_tomography param
    prepare_path param
    margi  = param[:data].to_marginal param
    s      = Time.new
    wigner = tomography margi, param
    puts "reconstruction done (#{(Time.new-s)} sec)"
    wigner.renorm if param[:renorm]
    results = [margi, wigner]
    results << wigner.photons(param[:photon]) if param[:p_distrib]
    results
  end

  def tomography margi, param
    grid   = Quix::G.new( param[:grid_typ], param[:grid_max], param[:grid_pts], param[:grid_pts])
    case param[:algorithm]
      when :irt     then margi.tomography( :kirt, grid, param[:cutoff] )   
      when :kirt    then margi.tomography( :kirt, grid, param[:cutoff] )
      when :sirt    then margi.tomography( :sirt, grid )
      when :maxlik  
        margi.tomography( 
          :maxlik, 
          {iterations: param[:iteration], photons: param[:photon], report: 10} 
        ).wigner(grid)
    end
  end

  def make_margi phase, quad, num
    Quix::Marginal.new( Q::marginal_alloc(num) ).tap do |x|
      x[:phase].put_array_of_double 0, phase[0..(num-1)]
      x[:quad].put_array_of_double 0, quad[0..(num-1)]
    end
  end

end
