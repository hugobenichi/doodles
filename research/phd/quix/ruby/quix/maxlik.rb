module Quix

  module Maxlik

    Quix::todo self, "check SegFault/BusError when doing multithreaded maxlik iter"

    extend Quix_Class

    [
      [:m2dmatrix,      [:pointer],                 :pointer],
      [:h2dmatrix,      [:pointer],                 :pointer],
      [:m2diag,         [:pointer,:pointer],        :void],
      #[:h2diag,         [:pointer,:pointer],        :void],
    ].each{ |sig| attach_quix_function sig }

  end  

  class Maxlik_Dmatrix < FFI::ManagedStruct

    include Quix_Object

    [
      [:new,            [],               :pointer],
      [:free,           [:pointer],       :void],
    ].each{ |sig| attach_quix_function sig }

    layout  :report,      :int,
            :iterations,  :int,
            :photons,     :int,
            :n_thread,    :int,
            :distances,   :pointer,
            :data,        :pointer,
            :maxlik_cb,   :pointer

    def initialize args
      case args
        when FFI::Pointer then super args
        when Hash
          super Q::maxlik_dmatrix_new    
          self[:report]     = args[:report]       || 0
          self[:iterations] = args[:iterations]   || 50
          self[:photons]    = args[:photons]      || 15
          self[:n_thread]   = args[:n_thread]     || 0
          self[:distances]  = args[:distances]    || nil
          self[:data]       = args[:data].pointer || nil
          self[:maxlik_cb]  = args[:maxlik_cb]    || nil
          @type = args[:data]  
      end
    end

    def calculate
      Q::DMatrix.new case @type
        when Marginal  then Q::maxlik_m2dmatrix self
        when Histogram then Q::maxlik_h2dmatrix self
      end
    end

    def distances
      self[:distances].get_array_of_double(0, self[:iterations])
    end

    def diag
      m = self[:photons]
      Q::Photon_Distribution.new FFI::MemoryPointer.new(:double, m).tap{ |p|
        case @type
          when Marginal  then Q::maxlik_m2diag self, p
       #  when Histogram then Q::maxlik_h2diag self, p
        end
      }.get_array_of_double(0, m)
    end

  end

  class Marginal < FFI::ManagedStruct
    include Quix_Tomography

    def maxlik args = {}
      args[:data] = self
      Q::Maxlik_Dmatrix.new(args).calculate 
    end
    self.add_algorithm :maxlik, :maxlik

    def maxlik_diag args = {}
      args[:data] = self
      Q::Maxlik_Dmatrix.new(args).diag
    end
    self.add_algorithm :diag, :maxlik_diag

  end

  Quix.todo self, "add maxlik histogram 2 dmatrix"
  class Histogram < FFI::ManagedStruct
    include Quix_Tomography
    def maxlik args = {}
      args[:data] = self
      Q::Maxlik_Dmatrix.new(args).calculate 
    end
    self.add_algorithm :maxlik, :maxlik
  end

end
