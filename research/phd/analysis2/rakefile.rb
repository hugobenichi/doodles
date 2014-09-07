require "rake/clean"

module Compile

  @directory   = File.expand_path(File.dirname(__FILE__))
  @module_list = []
  @object_list = []
  @compiler    = ENV.has_key?("OS") ? "gcc44 " : "gcc "

  class << self

    attr_reader :module_list, :object_list

    def dir
      @directory + "/c/"
    end

    def run command
      sh @compiler + command
    end

    def flag_c
      "-O2 -ffast-math -march=native -I#{dir}"
    end

    def flag_lib
      "-lm"
    end

    def library name
      @directory + "/lib/" + (ENV.has_key?("OS") ? "#{name}32.dll" : "lib#{name}.so")
    end

    def to_compile symbol, target, info = ""
      desc info
      task symbol => target
      @module_list << symbol
    end

    def to_link o
      @object_list << o
    end

    def add_module( name, input, info = "")
      output = []
      mod = namespace(name) do
        case input[-2]
          when '.'
            [dir + input]
          else
            FileList[dir + input + '*.c']
        end.each do |s| 
          o = s.gsub(%r|./.*/|,"./").gsub(".c",".o")
          o = s.gsub(".c",".o")
          to_link o
          CLEAN.include o
          desc "compiling #{s}"
          file(o => s) do run "-c -o %s %s %s" % [o,s,flag_c] end
          output << o
        end
      end
      to_compile name, output.map{|o| mod[o]}, info
    end

  end

end


Compile::add_module :frames, 'frames/', "compiling frames module"


analysis = namespace :analysis do

  lib  = Compile.library "analysis"
  objs = Compile.object_list.join " "

  task :library => lib

  desc 'compiling main library'
  file lib => Compile.object_list do
    Compile.run "-shared -o %s %s %s" % [ lib, objs, Compile.flag_lib ]
  end

end

Compile::to_compile :analysis, analysis[:library], "main analysis library"


desc 'Compiling all source files'
task :all => Compile.module_list


desc 'listing all source compiling scripts'
task :list do puts 'list of all compilation modules','', Compile.module_list,'' end


task :default   => [:list]
task :recompile => [:clean, :analysis] 

