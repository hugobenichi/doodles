module Quix

  class Generator < FFI::ManagedStruct

    include Quix_Object
    
    [
      [:make_wigner,    [:pointer,:pointer],                :pointer],
      [:make_dmatrix,   [:pointer,:int],                    :pointer],
      [:make_histogram, [:pointer,:pointer,:pointer],       :pointer],
      [:make_marginal,  [:pointer,:int, :double],           :pointer],
      [:free,           [:pointer],                         :void],
      [:vacuum,         [],                                 :pointer],
      [:vac_tele,       [:double],                          :pointer],
      [:thermal,        [:double],                          :pointer],
      [:thm_tele,       [:double,:double],                  :pointer],
      [:coherent,       [:double,:double],                  :pointer],
      [:coh_tele,       [:double,:double,:double],          :pointer],
      [:coh_loss_tele,  [:double,:double,:double,:double],  :pointer],
      [:squeezing,      [:double],                          :pointer],
      [:sqz_tele,       [:double,:double],                  :pointer],
      [:sqz_loss_tele,  [:double,:double,:double],          :pointer],
      [:cat,            [:double,:double,:double],          :pointer],
      [:one,            [],                                 :pointer],
      [:one_loss,       [:double],                          :pointer],
      [:one_tele,       [:double],                          :pointer],
      [:sqzd_photon,    [:double],                          :pointer],
    ].each{ |sig| attach_quix_function sig }  
  
    layout  :parameters,    :pointer,
            :g_wig,         :pointer,
            :dmat,          :pointer

    def initialize *args
      super Quix.send "generator_#{args[0]}".to_s, *args[1..-1] 
    end

    def wigner gd 
      Quix::Wigner.new self.make_wigner gd
    end

    def marginal n = 50000, l = 8.0
      Quix::Marginal.new self.make_marginal n, l
    end

    #def histogram grid, range
    #  Quix::Histogram.new self.make_histogram grid, range
    #end

    def histogram grid, n = 50000
      marginal(n).to_h grid
    end

    def dmatrix n = 15
      Quix::DMatrix.new self.make_dmatrix n
    end

    class << self
      [
        :vacuum,        :vac_tele, 
        :thermal,       :thm_tele, 
        :coherent,      :coh_tele,    :coh_loss_tele, 
        :squeezing,     :sqz_tele,    :sqz_loss_tele, 
        :cat, 
        :one,           :one_loss,    :one_tele,
        :sqzd_photon, 
      ].each{ |sig| define_method(sig){ |*args| Generator.new sig, *args } }
    end

    Quix.todo self, "add default constructor value for Class function constructors"

  end

  Gen = Generator

end
