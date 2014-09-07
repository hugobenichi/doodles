
name = "c_buffer"

lib_type = ENV["OS"] ? "dll" : "so"
lib_file = "./lib/%s/%s.%s" % [name,name,lib_type]
src_file = "./ext/%s/%s.c"  % [name,name]
hdr_path = "./ext/%s"       % name

flags = ENV["OS"] ? "-D _WIN32" : "-fPIC"

task :compile => lib_file

task lib_file => src_file do
  sh "gcc -shared -o #{lib_file} #{src_file} -I#{hdr_path} #{flags}"
end

task :test_global do
  ruby "test/test_%s.rb" % name
end

task :test_local do
  ruby "-I./lib test/test_%s.rb" % name
end

task :gem_install => :gem_build do
  gemfile = Dir.new("./").entries.select{ |f| f =~ /c_buffer-[\d]+\.[\d]+\.[\d]+.gem/ }[0]
  puts "installing %s" % gemfile
  sh "gem install --local %s" % gemfile
end

task :gem_build do
  sh "gem build %s.gemspec" % name
end
