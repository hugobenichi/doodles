require 'ant'
require 'pathname'

project_dir = Pathname.new(__FILE__).dirname
class_dir = project_dir.to_path + '/build'

Dir.chdir project_dir     # unless ng server runs in same dir
$CLASSPATH << class_dir   # add compile classes directory to java classpath

dir = {
  :src        => 'src',
  :srcfiles   => FileList['src/**/*.java'],
  :build      => 'build',
  :classes    => 'build/classes',
  :doc        => 'build/doc'
}

compile_options = {
  :srcdir   => dir[:src],
  :destdir  => dir[:classes],
  :source   => '1.8',
  :target   => '1.8'
}

jarname = 'FooBarBaz.jar'

desc 'compile java sources'
task :build do
  ant.mkdir :dir => dir[:classes]
  ant.javac compile_options
end

desc 'run all tests'
task :test => :build do
  require 'test/foo_test-unit'
#  require 'test/foo_test-spec'
#  require 'test/foo_test-shoulda'
end

desc 'prepare a jar file for the project'
task :jar => :build do
  ant.jar :destfile => dir[:build] + '/' + jarname
end

desc 'generate javadoc'
task :doc do
  ant.javadoc :sourcefiles => dir[:srcfiles], :destdir => dir[:doc]
end

desc 'clean built classes files and jar artifacts'
task :clean do
  ant.delete :dir => dir[:build]
end
