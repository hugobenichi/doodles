

module SoL

  [
    'vector',
    'autowrapper',
    'algebra',
    'function',
    'reduce',
    'array'
  ].each { |mod| require './lib/sol/%s' % mod }

end


