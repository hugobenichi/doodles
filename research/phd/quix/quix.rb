

module Quix

  @todo = []

  class << self

    def n_thread
      4
    end

    def todo who, what
      @todo << [who, what]
    end

    def todo?
      puts "left to do in QUIX",""
      @todo.each{ |todo| puts todo[0],"  " + todo[1], "" }
    end

  end

  QDIR = File.expand_path(File.dirname(__FILE__)) unless defined? QDIR

  require "ffi"
  require QDIR + "/ruby/redplot/redplot"
  #require "redplot"

  extend FFI::Library
  ffi_lib  "%s/c/lib/quix/%s" % [ QDIR, ENV.has_key?("OS") ? "quix32.dll" : "libquix.so" ]
  
  module Quix_Object

    def method_missing name, *args
      Quix.send "#{self.class.quix_name}_#{name}".to_sym, self, *args
    end

    Quix.todo self, "check double autorelease"

    def self.included object
      object.extend Quix_Class
      object.class_eval do
        define_singleton_method :release do |ptr|
          unless object.name == "Quix::Range" || object.name == "Quix::Grid"
          #puts "Release object #{object} instance #{ptr}"
          Quix.send "#{quix_name}_free".to_sym, ptr
          end
        end
      end
    end    

  end

  module Quix_Class 
  
    def quix_name
      self.name.sub!(/[^ ]+::/,"").downcase
    end   

    def attach_quix_function sigs
      sigs[0] = ("%s_%s" % [quix_name, sigs[0]]).to_sym
      Quix::attach_function *sigs 
    end
    
  end

  module Quix_Tomography

    def tomography algorithm, *args
      self.send self.class.algorithm(algorithm), *args
    end
    alias_method :tomo, :tomography

    def self.included object
      object.class_eval do
        attr_reader :algorithms
        define_singleton_method :add_algorithm do |name, method|
          @algorithms ||= {}
          @algorithms[name] = method 
        end
        define_singleton_method :algorithm do |key|
          @algorithms[key]
        end
      end
    end 

  end

  module Quix_Negativity

    def negativity algorithm, *args
      self.send self.class.neg_algorithm(algorithm), *args
    end
    alias_method :neg, :negativity

    def self.included object
      object.instance_eval do
        attr_reader :neg_algorithms
        define_singleton_method :add_neg_algorithm do |name, method|
          @neg_algorithms ||= {}
          @neg_algorithms[name] = method 
        end
        define_singleton_method :neg_algorithm do |key|
          @neg_algorithms[key]
        end
      end
    end 

  end

  Quix::todo self, "Refactor modules for dynamically adding tomography algorithms"

  module Quix_Negvar

    def negvar algorithm, *args
      self.send self.class.negvar_algorithm(algorithm), *args
    end

    def self.included object
      object.instance_eval do
        attr_reader :negvar_algorithms
        define_singleton_method :add_negvar_algorithm do |name, method|
          @negvar_algorithms ||= {}
          @negvar_algorithms[name] = method 
        end
        define_singleton_method :negvar_algorithm do |key|
          @negvar_algorithms[key]
        end
      end
    end 

  end

  module Quix_MonteCarlo

    def mc_negvar algorithm, *args
      self.send self.class.mc_negvar_algorithm(algorithm), *args
    end

    def self.included object
      object.instance_eval do
        attr_reader :mc_negvar_algorithms
        define_singleton_method :add_mc_negvar_algorithm do |name, method|
          @mc_negvar_algorithms ||= {}
          @mc_negvar_algorithms[name] = method 
        end
        define_singleton_method :mc_negvar_algorithm do |key|
          @mc_negvar_algorithms[key]
        end
      end
    end 

  end

  module Quix_Memory

    extend Quix_Class

    [
      [:set_allocator,      [:pointer],   :void],
      [:set_desallocator,   [:pointer],   :void],    
    ].each{ |sig| attach_quix_function sig }  
    
    all_pointer = []

    Allocator = FFI::Function.new(:void, [:int]) do |address|
      #puts "adding address %i to memory bank" % address
      all_pointer << address
      nil
    end 

    Desallocator = FFI::Function.new(:int, [:int]) do |address|
      if all_pointer.delete(address)
        #puts "removing address %i from memory bank" % address
        1
      else
        #puts "address %i already removed" % address
        0
      end
      #all_pointer.delete(address) ? 1 : 0
    end     

    Quix::quix_memory_set_allocator     Allocator
    Quix::quix_memory_set_desallocator  Desallocator

  end

  
  Dir.new(QDIR+"/ruby/quix").each{ |mod| require "%s/ruby/quix/%s" % [QDIR,mod] if mod.include? ".rb" }

end


Q = Quix

#require "./quix_test.rb"



