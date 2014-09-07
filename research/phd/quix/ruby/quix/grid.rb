

module Quix

  class Range < FFI::ManagedStruct
    
    include Quix_Object
    
    [
      [:new,            [:int],                   :pointer],
      [:free,           [:pointer],               :void],
      [:single,         [:int,:double],           :pointer],
      [:dual,           [:int,:double],           :pointer],
    ].each{ |sig| attach_quix_function sig }

    layout  :resolution,  :double,
            :amplitude,   :double,
            :n_val,       :int,
            :val,         :pointer

    def val
      @val ||= self[:val].get_array_of_double( 0, self[:n_val] )
    end

    def each
      val.each{ |x| yield(x) }
    end

    def [](index)
      case index
        when Symbol then super index
        when Fixnum then self.val[index]
      end
    end

  end

  R = Range

  Grid_type = enum(
    :cartesian,
    :radial,
    :histo
  )

  class Grid < FFI::ManagedStruct

    include Quix_Object

    [
      [:xy,             [:double,:int],           :pointer],
      [:rad,            [:double,:int,:int],      :pointer],
      [:free,           [:pointer],               :void],
      [:hist,           [:pointer],               :void],
      [:cell,           [:pointer],               :double],
    ].each{ |sig| attach_quix_function sig }

    layout  :x,           :pointer,
            :y,           :pointer,
            :type,        Grid_type

    def initialize *args
      super case args[0]
        when FFI::Pointer then args[0]
        when :xy          then Quix::grid_xy *args[1..2] 
        when :rad         then Quix::grid_rad *args[1..3] 
      end
    end

    def each
      x.each{ |a| y.each{ |b| yield(a,b) } }
    end

    def x
      @x ||= Range.new self[:x]
    end

    def y
      @y ||= Range.new self[:y]
    end

    def val
      @val ||= x.val.map{ |v_x| y.val.map{ |v_y| [v_x, v_y] } }
    end

    def [](index)
      case index
        when Symbol then super index
        when Fixnum then self.val[index]
      end
    end

    def cartesian?
      self[:type] == :cartesian
    end

    def radial?
      self[:type] == :radial
    end

    def histogram?
      self[:type] == :histo
    end

    def to_h!
      self.hist
      self
    end

    Quix.todo self, "add to_h method which copies a grid object"    

    def theta
      return nil if cartesian?
      return x   if histogram?
      return y   if radial?
    end

    def r
      return nil if cartesian?
      return y   if histogram?
      return x   if radial?
    end
    
    class << self
    
      [:xy, :rad].each do |typ|
        define_method(typ){ |*args| Grid.new typ, *args }
      end
    
    end

  end
  
  G = Grid

end
