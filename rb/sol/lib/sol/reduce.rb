

module SoL

  #for all operations which are linear / arity 1 | 2
  module Reduce

    [
      'sum',
      'prod',
      'l1',
      'l2',
      'max',
      'min'
    ].each do |func|
      define_method(func.to_sym) do
        ptr = FFI::MemoryPointer.new( self.type )
        self.hook.send("reduce1_#{func}".to_sym, self.mem, ptr, self.size)
        ptr.get_double(0)
      end
    end

    [
      'dot',
      'l1',
      'l2'
    ].each do |func|
      define_method(func.to_sym) do |other|
        ptr = FFI::MemoryPointer.new( self.type )
        self.hook.send("reduce2_#{func}".to_sym, self.mem, other.mem, ptr, self.size)
        ptr.get_double(0)
      end
    end

  end

  class Vector
    include Reduce
  end

end



