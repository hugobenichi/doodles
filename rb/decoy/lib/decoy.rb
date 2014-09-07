require 'ffi'

Decoy = Module.new do
  extend FFI::Library
  ffi_lib $LOAD_PATH.map{ |path| path + "/decoy/decoy.so" }
  
  attach_function :decoy_print, [:int,:int,:int], :void

  class << self
    def sing
      puts "cui cui"
    end
    def conf   
      Dir.chdir( File.dirname(__FILE__) + "/../") do
        require 'rake'
        sh 'rake pre_install'          
      end
    end
  end
  
end


