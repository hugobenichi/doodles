

module Compile

  @directory   = File.expand_path(File.dirname(__FILE__))
  @module_list = []
  @object_list = []
  @compiler    = ENV.has_key?("OS") ? "gcc44 " : "gcc "

  class << self

    attr_reader :module_list, :object_list

    def dir_quix
      "./quix/"
    end

    def run command
      sh @compiler + command
    end

    def flag_c
      "-O2 -ffast-math -fgcse-lm -fgcse-sm -mfpmath=387 -ftree-vectorize -march=native -fstrict-aliasing -std=c99 -I#{@directory}"
      "-O2 -ffast-math -mfpmath=387 -march=native -I#{@directory}"
    end

    def flag_lib
      if ENV.has_key?("OS")
        "-lm -lgsl -lgslcblas -L./lib/gsl"
      else
        #"-lc -lm -lgsl -lgslcblas -lpthread -L./lib/gsl"
        "-lm -lgsl -lgslcblas -lpthread -L./lib/gsl"
      end
    end

    def library name
      "./lib/quix/" + (ENV.has_key?("OS") ? "#{name}32.dll" : "lib#{name}.so")
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
            [dir_quix + input]
          else
            FileList[dir_quix + input + '*.c']
        end.each do |s| 
          o = s.gsub(%r|./.*/|,"./").gsub(".c",".o")
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


FileList['./rakelib/*.rb'].each{ |mod| require mod }


quix = namespace :quix do

  lib  = Compile.library "quix"
  objs = Compile.object_list.join " "

  task :library => lib

  desc 'compiling main library'
  file lib => Compile.object_list do
    Compile.run "-shared -o %s %s %s" % [ lib, objs, Compile.flag_lib ]
  end

end

Compile::to_compile :quix, quix[:library], "main quix library"


desc 'Compiling all source files'
task :all => Compile.module_list


desc 'listing all source compiling scripts'
task :list do puts 'list of all compilation modules','', Compile.module_list,'' end


task :default => [:list]
task :recompile => [:clean, :quix]

