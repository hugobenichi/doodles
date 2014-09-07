module Quix
  
  class Irt_Negvar < FFI::ManagedStruct
  
    include Quix_Object   
  
    [
      [:new,              [:int],             :pointer],
      [:free,             [:pointer],         :void],
    ].each{ |sig| attach_quix_function sig }  

    layout  :n,           :int,
            :dispersion,  :pointer,
            :negativity,  :pointer,
            :variance,    :pointer

    def dispersion
      self[:dispersion].get_array_of_double( 0, self[:n] )
    end

    def negativity
      self[:negativity].get_array_of_double( 0, self[:n] )
    end

    def variance
      self[:variance].get_array_of_double( 0, self[:n] )
    end


if false
    def header
      [
        "unset title",
        'set log xy',
        'set xlabel "iteration"',
        'set ylabel "dispersion"',
        'plot "-" w l t "dispersion", "" w l t "pt-wise variance"' 
        #'plot "-" w l title "dispersion", "-" w l title "negativity", "-" w l, "" w yerrorbars' 
      ]
    end

    def todraw
      return Array.new(self[:n]){|i|i}, self.dispersion, self.variance
    end
end

  end

end
