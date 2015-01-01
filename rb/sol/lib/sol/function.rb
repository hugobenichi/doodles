

module SoL

  #for all operations which are linear / arity 1
  module Function

    [
      'log',
      'log2',
      'log10',
      'cos',
      'sin',
      'tan',
      'acos',
      'asin',
      'atan',
      'cosh',
      'sinh',
      'tanh',
      'acosh',
      'asinh',
      'atanh',
      'erf',
      'sqr',
      'isqr',
      'sqrt',
      'isqrt',
      'cub',
      'icub',
      'inv',
      'abs',
    ].each do |func|
      define_method(func.to_sym) do
        self.hook.send("linear1_#{func}".to_sym, self.mem, self.mem, self.size)
        self
      end
    end

  end

  class Vector
    include Function
  end

end



