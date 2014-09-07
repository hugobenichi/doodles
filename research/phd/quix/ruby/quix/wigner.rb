module Quix

  class Wigner < FFI::ManagedStruct

    RedPlot::insert(in: self, as: :plot, command: 'splot') do
      set.xlabel '"q"'
      set.ylabel '"p"'
      set.ticslevel 0
      set.xrange "[-4.0:4.0]"
      set.yrange "[-4.0:4.0]"
      set.zrange "[-0.35:0.35]"
      set.cbrange "[-0.35:0.35]"
      unset.colorbox
      unset.title
      set.view "70,70"
      set.pm3d.depthorder.hidden3d 2
      set.style.line(2).linetype(7).linewidth 0.6
      set.style.fill.transparent.solid(0.30).border
    end

    include Quix_Object

    [
      [:new,            [:pointer],                   :pointer],
      [:free,           [:pointer],                   :void],
      [:to_zero,        [:pointer],                   :void],
      [:norm,           [:pointer],                   :double],
      [:renorm,         [:pointer],                   :void],
      [:purity,         [:pointer],                   :double],
      [:ripple,         [:pointer,:double],           :double],
      [:max,            [:pointer],                   :double],
      [:min,            [:pointer],                   :double],
      [:overlap,        [:pointer,:pointer],          :double],
      [:dist_over,      [:pointer,:pointer],          :double],
      [:dist_l2,        [:pointer,:pointer],          :double],
      [:x_marg,         [:pointer],                   :pointer],
      [:p_marg,         [:pointer],                   :pointer],
      [:linear,         [:pointer,:pointer,:int],     :pointer],
      [:convolate,      [:pointer,:pointer],          :pointer],
      [:gaussian_conv,  [:pointer,:double],           :void],
      [:loss,           [:pointer,:double],           :void],
      [:teleport,       [:pointer,:double,:pointer],  :pointer],
      [:teleport_neg,   [:pointer,:double],           :double],
    ].each{ |sig| attach_quix_function sig }

    layout  :gd,          :pointer,
            :val,         :pointer

    def initialize arg

      plot.todraw.add('with pm3d notitle') do
        x, y, z = [], [], []
        if grid.cartesian?
          z = Array.new(val).map{ |col| Array.new(col) << "" }.flatten
          grid.x.each do |a| 
            grid.y.each do |b|
              x << a
              y << b
            end
            x << ""
            y << ""
          end
        else
          z = Array.new(val.transpose).map{ |col| Array.new(col) << "" }.flatten
          z_len = z.length
          grid.y.each do |b|
            grid.x.each do |a| 
              x << a * Math::cos(b)
              y << a * Math::sin(b)
            end
            x << ""
            y << ""
          end          
          b = grid.y.val[0]
          grid.x.each do |a| 
            x << a * Math::cos(b)
            y << a * Math::sin(b)
            z << z[-z_len]
          end
        end
        [x, y, z]
      end

      super case arg
        when FFI::Pointer then arg
        when Grid         then Quix::wigner_new arg
      end

    end

    def grid
      @grid ||= Grid.new self[:gd]
    end

    def val
      @val ||= self[:val].get_array_of_pointer(0, grid.x.val.length).map! do |col|
        col.get_array_of_double 0, grid.y.val.length
      end
    end

    def [](index)
      case index
        when Symbol then super index
        when Fixnum then self.val[index]
      end
    end

    def put_val val
      self[:val].get_array_of_pointer(0, val.length).each_with_index do |ptr,i|
        ptr.put_array_of_double 0, val[i]
      end
    end

    def dmatrix n
      Fock.new(grid, n).dmatrix self
    end

    def photon_distribution n
      fock = Q::Fock.new self.grid, n
      Q::Photon_Distribution.new Array.new(n){ |i| overlap fock[i][i].re }
    end

    def norm
      Quix::wigner_norm self
    end

    def neg
      return val[0][0] if grid.radial?
      l_x, l_y = val.length, val[0].length
      val[l_x/2][l_y/2]
    end

    alias_method :photons, :photon_distribution


    Quix::todo self, "check Wigner::from file"

# !!                            !!
# !!  ERROR IN GRID RESOLUTION  !!
# !!                            !!
    def self.from_file path  
      data = File.open(path,"r").readlines.delete_if{ |line| line.length < 5}
      len = data.length

      x, y, val = Array.new(len), Array.new(len), Array.new(len) 
      len.times do |i| 
        x[i], y[i], val[i] = data[i].split(" ").map{|x| x.to_f} 
      end
      
      xx, yy = [], []
      x.each_index { |i| xx << x[i] unless x[i] == x[i-1] }  
      yy << y.shift until yy[0] == y[0]

      g = if xx == yy
            Grid.new :xy, xx[-1], (xx.length+1)/2
          else
            Grid.new :rad, xx[-1], xx.length, yy.length
          end

      rng_x, rng_y = g.x, g.y
      rng_x[:val].put_array_of_double 0, xx
      rng_y[:val].put_array_of_double 0, yy

      w_val, w  = [], Wigner.new(g)
      i, stride = 0, xx.length
      while i < val.length
        w_val << val[i..i+stride-1]
        i += stride
      end 

      w[:val].get_array_of_pointer(0, xx.length).each do |ptr|
        ptr.put_array_of_double 0, w_val.shift
      end

      return w

    end

  end

  W = Wigner

  class Wigner_Comp < FFI::ManagedStruct

    include Quix_Object

    [
      [:new,            [:pointer],                   :pointer],
      [:free,           [:pointer],                   :void],
      [:downconvert,    [:pointer],                   :pointer],
      [:convert,        [:pointer],                   :pointer],
    ].each{ |sig| attach_quix_function sig }

    layout  :gd,          :pointer,
            :re,          :pointer,
            :im,          :pointer

    def initialize arg
      super case arg
        when FFI::Pointer then arg
        when Grid         then Quix::wigner_comp_new arg
      end
    end

    def re
      @ptr ||= self.downconvert
      @re  ||= Q::W.new @ptr.get_array_of_pointer(0,2)[0] 
    end

    def im
      @ptr ||= self.downconvert
      @im  ||= Q::W.new @ptr.get_array_of_pointer(0,2)[1] 
    end

  end

end


