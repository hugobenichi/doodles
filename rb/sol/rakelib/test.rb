

### TESTS ###


namespace :test do

  task :link do 
    ['ruby','jruby','rbx'].each do |eng| sh eng + ' ./test/test_simple.rb' end
  end

  task :unit do
    sh 'ruby ./test/test_sol.rb'
  end

  task :all => [:link,:unit]

end


task :test => 'test:all'


