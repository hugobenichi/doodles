
File.open('generate.c','w') do |file|

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
  ].each do |func|

    file.puts "#define spec_%s(X,Y)        Y = %s(X)" % [func,func]

  end

end
