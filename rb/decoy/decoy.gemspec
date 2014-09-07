#!/usr/local/bin/ruby -S gem build

Gem::Specification.new do |spec|

  spec.name        = 'decoy'
  spec.version     = '0.0.1'
  spec.date        = '2012-06-05'
  spec.summary     = "Testing gemspec C extention building"
  spec.description = "trolololo"
  spec.authors     = ["Hugo Benichi"]
  spec.email       = 'hugo.benichi@m4x.org'
  
  spec.files       = Dir.glob( 'lib/**/*.rb') + Dir.glob( 'ext/**/*.{c,h,rb}' )  
  spec.files       << 'rakefile.rb'
  
  spec.extensions  = ['ext/decoy/extconf.rb']  
  
end
  

je prepare les progs pour l acquisition/analyse de donnees
