


flags = "-O3"
#flags << " -ffast-math -msse2 -mfpmath=387"
#flags << " -fstrict-aliasing -std=c99"
#flags << " -DUSE_RESTRICT"

file 'libloop.so' => ['loop.o','scalar.o'] do 
  sh "gcc -shared -o libloop.so loop.o scalar.o"  
end


file 'loop.o'     => 'loop.c'   do sh "gcc -c loop.c #{flags}"            end
file 'scalar.o'   => 'scalar.c' do sh "gcc -c scalar.c #{flags}"          end


task :run         => :compile do require './loop.rb' end
task :compile     => 'libloop.so' 
task :recomp      do
  sh "gcc -shared -o libloop.so loop.o"
  sh "gcc -c loop.c #{flags}"
end
task :default     => :compile



