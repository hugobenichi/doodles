

module SoL

  #for all operations which are linear / arity2 | hete
  module Albegra

    def +(other)    #first min of size, then include stride abd broadcast
      case other
      when SoL::Vector
        self.hook.linear2_plus( self.mem, other.mem, self.mem, self.size )
      when Numeric
        ptr = FFI::MemoryPointer.new( self.type).put_double( 0, other.to_f)
        self.hook.linear_hete_plus( self.mem, ptr, self.mem, self.size )
      end
      self
    end

  end

  class Vector
    include Albegra
  end

end



class Numeric

#  plus = '+'.to_sym

#  alias :plus_raw plus

#  def +(other)
#    case other
#    when SoL::Vector then other.+(self)
#    else self.plus_raw(other)
#    end
#  end

end



