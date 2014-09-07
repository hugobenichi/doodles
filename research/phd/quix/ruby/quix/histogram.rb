module Quix

  class Histogram < FFI::ManagedStruct

    RedPlot::insert(in: self, as: :plot, command: 'splot') do
      unset.title
      set.xlabel '"{/Symbol t}"'
      set.xlabel '"theta"'
      set.ylabel '"q"'
      set.ticslevel 0
      set.xrange "[0:2*pi]"
      set.yrange "[-5.0:5.0]"
    end

    include Quix_Object
      
    [
      [:new,            [:pointer],               :pointer],
      [:free,           [:pointer],               :void],
      [:normalize,      [:pointer],               :void],
    ].each{ |sig| attach_quix_function sig }
    
    layout  :grid,              :pointer,
            :val,               :pointer

    def initialize *args
      plot.todraw.add('with pm3d notitle') do
        x, y, z = [], [], Array.new(val).map{ |col| Array.new(col) << "" }.flatten
        grid.x.each do |a| 
          grid.y.each do |b|
            x << a
            y << b
          end
          x << ""
          y << ""
        end
        [x, y, z]
      end
      super *args
    end

    def grid
      @grid ||= Grid.new self[:grid]
    end

    def val
      @val ||= self[:val].get_array_of_pointer(0, grid.x.val.length).map! do |col|
        col.get_array_of_double 0, grid.y.val.length
      end
    end

    def norm
      self.normalize
      self
    end

    #def [](index)
    #  self.val[index]
    #end

    Quix.todo self, "add double value access with [][]"

  end

  Hist = Histogram  

end
