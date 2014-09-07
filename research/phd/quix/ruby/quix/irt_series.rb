module Quix
 
  module SIRT

    Quix.todo self, "implement series wig reconstruction for square grids"
  
    extend Quix_Class

    [
      [:calculate,                [:pointer,:pointer],                      :void],
      [:calculate_neg,            [:pointer,:pointer],                      :double],
      [:calc_neg_var,             [:pointer,:pointer],                      :double],
      [:calc_neg_var_conv,        [:pointer,:pointer,:pointer],             :pointer],    
      [:mc_m2neg_var,             [:int,:pointer,:pointer],                 :pointer],
      [:mc_m2negvar_wk,           [:int,:pointer,:pointer],                 :pointer],      
    ].each{ |sig| attach_quix_function sig }  

  end  


  class Marginal < FFI::ManagedStruct

    include Quix_Tomography
    def series_irt( grid, series = Series.new(8,40,10.0), n_thread = Quix::n_thread )
      series.calculate(self).wigner grid
    end
    add_algorithm :sirt, :series_irt

    include Quix_Negativity
    def series_negativity series = Series.new(8,40,10.0)
      Q::sirt_calculate_neg series, self
    end
    add_neg_algorithm :sirt, :series_negativity

    include Quix_Negvar
    def series_negvar series = Series.new(8,40,10.0)
      var = Q::sirt_calc_neg_var series, self
      neg = series.negativity
      [neg,var]
    end
    add_negvar_algorithm :sirt, :series_negvar

    include Quix_MonteCarlo
    def series_mc_negvar iteration, events, series = Series.new(8,40,10.0)
      get = FFI::Function.new(:pointer, []) { self.mc_bootstrap(events) }
      Quix::Irt_Negvar.new Quix::sirt_mc_m2negvar_wk(iteration, series, get)
    end
    add_mc_negvar_algorithm :sirt, :series_mc_negvar

  end


  Quix::todo self, "add histogram series irt"
  class Histogram < FFI::ManagedStruct

    include Quix_Tomography
    def series_irt cutoff, grid
    end

    include Quix_Negativity
    def series_negativity cutoff
    end

  end

  class Generator < FFI::ManagedStruct
    include Quix_MonteCarlo
    def series_mc_negvar iteration, events, series = Series.new(8,40,10.0), amplitude = 8.0
      get = FFI::Function.new(:pointer, []) { self.make_marginal events, amplitude }
      Quix::Irt_Negvar.new Quix::sirt_mc_m2negvar_wk(iteration, series, get)
    end
    add_mc_negvar_algorithm :sirt, :series_mc_negvar
  end


  class Series < FFI::ManagedStruct

    include Quix_Object

    [
      [:new,                  [:int,:int,:double],                      :pointer],
      [:free,                 [:pointer],                               :void],
      [:wigner,               [:pointer,:pointer],                      :pointer],
      [:negativity,           [:pointer],                               :double],
    ].each{ |sig| attach_quix_function sig }  

    layout  :siz_ang,   :int,
            :siz_rad,   :int,
            :amp_rad,   :double, 
            :c_re,      :pointer,
            :c_im,      :pointer

    def initialize *args
      @calc = false
      super case args[0]
        when FFI::Pointer then args[0]
        when Fixnum       then Quix::series_new *args
      end
    end

    def wigner grid
      Q::W.new Q::series_wigner(self, grid) if @calc && grid.radial?
    end

    def calculate margi
      unless @calc
        Q::sirt_calculate self, margi 
        @calc = true
      end
      self
    end

#    def neg_conv margi
#      n = self[:siz_rad]
#      self.calculate margi unless @calc
#      p_err = FFI::MemoryPointer.new(:double, n)
#      p_var = self.calculate_disp_neg_conv( margi, p_err)
#      [ p_err.get_array_of_double(0,n), p_var.get_array_of_double(0,n) ]
#    end

  end

end


