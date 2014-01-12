require 'ant'
require 'pathname'

project_dir = Pathname.new(__FILE__).dirname
class_dir = project_dir.to_path + '/build/classes'

# TODO: find a way to hook efficiently to nailgun jruby process
Dir.chdir project_dir     # unless ng server runs in same dir
$CLASSPATH << class_dir   # add compile classes directory to java classpath

# TODO: pass this as environment
dir = {
  :src        => 'src',
  :srcfiles   => FileList['src/**/*.java'],
  :build      => 'build',
  :classes    => 'build/classes',
  :doc        => 'build/doc'
}

# TODO: pass this as argument to build task
compile_options = {
  :srcdir   => dir[:src],
  :destdir  => dir[:classes],
  :source   => '1.8',
  :target   => '1.8'
}

# TODO: pass this as environment conf
jarname = 'FooBarBaz.jar'

desc 'compile java sources'
task :build do
  ant.mkdir :dir => dir[:classes]
  ant.javac compile_options
end

desc 'run all tests'
task :test => :build do
  # TODO: find a better way to run and rerun test instead of requiring
  require 'test/foo_test-unit'
#  require 'test/foo_test-spec'
#  require 'test/foo_test-shoulda'
end

# TODO: add jar versioning
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
