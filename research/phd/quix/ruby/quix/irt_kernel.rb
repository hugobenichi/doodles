module Quix
  
  module KIRT
  
    extend Quix_Class   
  
    [
      [:marginal,           [:pointer,:double,:pointer,:int],         :pointer],
      [:m2neg,              [:pointer,:double,:pointer],              :void],
      [:m2neg_var,          [:pointer,:double,:pointer,:pointer],     :void],
      [:histogram,          [:pointer,:double,:pointer,:int],         :pointer],
      [:h2neg,              [:pointer,:double,:pointer],              :void],
      [:h2neg_var,          [:pointer,:double,:pointer,:pointer],     :void],
      [:mc_h2neg_var,       [:int,:double,:pointer],                  :pointer],
      [:mc_m2neg_var,       [:int,:double,:pointer],                  :pointer],
      [:mc_g2neg_var,       [:int,:double,:pointer,:int,:double],     :pointer],
      [:mc_m2negvar_wk,     [:int,:double,:pointer],                  :pointer],
      [:mc_g2negvar_wk,     [:int,:double,:pointer,:int,:double],     :pointer],
    ].each{ |sig| attach_quix_function sig }  
  
  end

  class Marginal < FFI::ManagedStruct

    include Quix_Tomography
    def kernel_irt grid, cutoff = 7.0, n_thread = Quix::n_thread
      Q::W.new Q::kirt_marginal self, cutoff, grid, n_thread
    end
    add_algorithm :kirt, :kernel_irt

    include Quix_Negativity
    def kernel_negativity cutoff = 7.0
      ptr = FFI::MemoryPointer.new :double, 1
      Q::kirt_m2neg self, cutoff, ptr
      ptr.get_double 0
    end
    add_neg_algorithm :kirt, :kernel_negativity

    include Quix_Negvar
    def kernel_negvar cutoff = 7.0
      ptr_neg = FFI::MemoryPointer.new :double, 1
      ptr_var = FFI::MemoryPointer.new :double, 1
      Q::kirt_m2neg_var self, cutoff, ptr_neg, ptr_var
      [ptr_neg.get_double(0),ptr_var.get_double(0)]
    end
    add_negvar_algorithm :kirt, :kernel_negvar

    include Quix_MonteCarlo
    def kernel_mc_negvar iteration, events, cutoff = 7.0
      get = FFI::Function.new(:pointer, []) { self.mc_bootstrap(events) }
      Quix::Irt_Negvar.new Quix::kirt_mc_m2negvar_wk(iteration, cutoff, get)
    end
    add_mc_negvar_algorithm :kirt, :kernel_mc_negvar

  end

  Quix::todo self, "add histogram kernel irt"
  class Histogram < FFI::ManagedStruct

    include Quix_Tomography
    def kernel_irt( grid, cutoff = 7.0)
    end

    include Quix_Negativity
    def kernel_negativity( cutoff = 7.0 )
    end

    include Quix_Negvar
    def kernel_negvar( cutoff = 7.0)
    end

  end

  class Generator < FFI::ManagedStruct
    include Quix_MonteCarlo
    def kernel_mc_negvar iteration, events, cutoff = 7.0, amplitude = 8.0
      get = FFI::Function.new(:pointer, []) { self.make_marginal events, amplitude }
      Quix::Irt_Negvar.new Quix::kirt_mc_m2negvar_wk(iteration, cutoff, get)
    end
    add_mc_negvar_algorithm :kirt, :kernel_mc_negvar

  end

end
