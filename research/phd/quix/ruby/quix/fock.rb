module Quix

  class Fock < FFI::ManagedStruct

    include Quix_Object  
  
    [
      [:new,            [:pointer,:int],          :pointer],
      [:free,           [:pointer],               :void],
      [:calculate,      [:pointer],               :void],
      [:wig_from_matrix,[:pointer,:pointer],      :pointer],
      [:wig_to_matrix,  [:pointer,:pointer],      :pointer],
    ].each{ |sig| attach_quix_function sig }  
  
    layout  :gd,               :pointer,
            :N,                :int,
            :photons,          :pointer

    def initialize grid, n = 15
      f = super Quix::fock_new grid, n
      f.calculate
      f
    end

    def wigner dmatrix
      W.new wig_from_matrix dmatrix
    end

    def dmatrix wigner 
      DMatrix.new wig_to_matrix wigner
    end

    def grid
      @grid ||= Grid.new self[:gd]
    end

    def wigners
      @wigners ||= self[:photons].get_array_of_pointer(0, self[:N]).map do |p_ary|
        p_ary.get_array_of_pointer(0, self[:N]).map do |p|
          Q::Wigner_Comp.new p
        end
      end
    end

    def [](index)
      case index
        when Symbol then super index
        when Fixnum then wigners[index]
      end
    end

  end

  class Photon_Distribution

    attr_reader :val

    def initialize distribution
      @val = distribution
      plot.header << "set xrange [-0.1:#{@val.length}.5]"
      plot.todraw.add("w histogram title 'photon distribution'"){ distribution }
    end

    RedPlot::insert(in: self, as: :plot, command: 'plot') do
      set.xlabel '"n"'
      set.ylabel '"p_n"'
      set.yrange "[-0.005:1]"
      set.xtics(1).offset(0.2)
      set.style.fill.solid.border -1
    end

    def [](index)
      @val[index]
    end

    def each
      @val.each{ |v| yield(v) }
    end
  
    def length
      @val.length
    end

    def dmatrix
      Q::DMatrix.from_diag self
    end

    def wigner grd
      Q::DMatrix.from_diag(self).wigner grd
    end

    include Enumerable

  end

end
