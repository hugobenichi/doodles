

dir_rake = './rakelib/'

Dir.new(dir_rake).grep(/.*rb/).each { |tasks| require dir_rake + tasks }


task :default     => 'compile:all'
task :all         => [:default, 'test:all', :tree, :doc]
task :doc         do sh 'rdoc sol' end


