

Gem::Specification.new do |spec|

  spec.name        = 'c_buffer'
  spec.version     = '0.1.4'
  spec.date        = '2012-07-09'
  spec.summary     = "A simple circular buffer and its Ruby interface"
  spec.description = "A simple circular buffer and its Ruby interface to go along RbScope and RbVisa. Perfect for staging high throughput input data to low throughput I/O."
  spec.authors     = ["Hugo Benichi"]
  spec.email       = 'hugo.benichi@m4x.org'
  spec.homepage    = "http://github.com/hugobenichi/cbuffer"
  
  #spec.files       = Dir.glob( 'lib/**/*.{rb,so,dll}') 
  #spec.files       = Dir.glob( 'lib/**/*.{rb}')   
  spec.files      += Dir.glob( 'ext/**/*.{c,h,rb}')
  spec.files      << 'rakefile.rb'
  spec.files      << 'README'
  spec.files      << 'test/test_c_buffer.rb'
  
  spec.add_dependency 'ffi'
  
  spec.extensions  = 'ext/c_buffer/extconf.rb'
  
end
  


