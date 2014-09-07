module Quix

  class Marginal < FFI::ManagedStruct

    RedPlot::insert(in: self, as: :plot, command: 'plot') do
      unset.title
      set.xlabel '"{/Symbol q}"'
      set.ylabel '"q"'
      set.xrange "[0:2*pi]"
      set.yrange "[-5.0:5.0]"
    end

    include Quix_Object
      
    [
      [:new,              [:int],                           :pointer],
      [:copy,             [:int,:pointer,:pointer],         :pointer],
      [:free,             [:pointer],                       :void],
      [:to_raw_histogram, [:pointer,:pointer],              :pointer],
      [:draw_gaussian,    [:double],                        :double],
      [:phase_noise,      [:pointer,:double],               :void],
      [:quad_noise,       [:pointer,:double],               :void],
      [:teleportation,    [:pointer,:double],               :pointer],
      [:loss,             [:pointer,:double],               :pointer],
      [:mc_bootstrap,     [:pointer,:int],                  :pointer],
      [:mc_proba,         [:pointer, :pointer,:int,:double,:double],  :void],
      [:mc_proba_to_quad, [:pointer, :pointer,:int,:double,:double],  :void],
      [:mc_histogram,     [:pointer,:int,:double],          :pointer],
    ].each{ |sig| attach_quix_function sig }
    
    layout  :samples,           :int,
            :quad,              :pointer,
            :phase,             :pointer

    class << self

      def from_file path, n = nil
        puts "building marginal distribution\n", "from file data in #{path} ...", n
        data = File.open(path,"r").readlines[0..-2].map{ |s| 
          s.split(" ").map!{ |x| x.to_f } 
        }
        Q::Marginal.new *data.transpose[0..1], n
      end

      alias_method :read, :from_file

      def gaussian n, sigma = 1.0/Math::sqrt(2)
        Array.new(n){ Quix::marginal_draw_gaussian(sigma)}
      end

    end

    def initialize *args

      plot.todraw.add('with dots notitle') do
        [phase,quad]
      end

      case args[0]
        when FFI::Pointer then super args[0]
        else
          n = args[0].length
          n = args[2] if args[2]
          super Q::marginal_new n
          self[:phase].put_array_of_double( 0, @phase = args[0][0..n-1] )
          self[:quad].put_array_of_double( 0,  @quad  = args[1][0..n-1] )
      end

    end

    def rewind
      @quad, @phase = nil, nil
    end

    def quad
      @quad ||= self[:quad].get_array_of_double 0, self[:samples]
    end

    def phase
      @phase ||= self[:phase].get_array_of_double 0, self[:samples]
    end

    Quix.todo self, "check marginal_draw_gaussian !!!"

    Quix.todo self, "add tele! and loss! methods without creating new objet"

    def teleportation r
      Q::Marginal.new Q::marginal_teleportation(self,r)
    end

    alias_method :tele, :teleportation

    def loss eta
      Q::Marginal.new Q::marginal_loss(self,eta)
    end

    def +(m)
      Q::Marginal.new( self.phase, [self.quad,m.quad].transpose.map!{|x,y| x + y} )      
    end

    def -(m)
      Q::Marginal.new( self.phase, [self.quad,m.quad].transpose.map!{|x,y| x - y} )      
    end

    def *(alpha)
      Q::Marginal.new( self.phase, self.quad.map{|x| alpha * x} )   
    end

    Quix.todo self, "checker to_raw_histogram c code in object/marginal.c"

    def to_h grid
      Q::Histogram.new( self.to_raw_histogram(grid) ).norm
    end

    alias_method :histrogram, :to_h

    Quix.todo self, "checker tomography methods and bootstrap/resampling"
    
    def bootstrap size = -1
      size = self[:samples]  if size < 100
      Q::Marginal.new self.mc_bootstrap size
    end

    def resample evs
      m = Q::Marginal.new Q::marginal_mc_bootstrap self, evs
      return yield m if block_given?
      return m
    end

  end

  M = Marginal

end
