

Gem::Specification.new do |s|
  s.name        = 'redplot'
  s.version     = '0.1.0'
  s.date        = '2012-06-05'
  s.summary     = "A simple layer wrapping around a gnuplot process."
  s.description = "redplot gives convenience handler methods from and to Ruby objects to easily communicate with a gnuplot process"
  s.authors     = ["Hugo Benichi"]
  s.email       = 'hugo.benichi@m4x.org'
  s.files       = ["lib/redplot.rb"]
  s.homepage    = 'https://github.com/hugobenichi/redplot'
  
  s.add_development_dependency "shoulda"
  #s.add_runtime_dependency "something"
end
