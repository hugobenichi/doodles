##
 #  
 #                          CBuffer 
 #    
 #  Simple native memory circular buffer for staging data before writing to disk
 #  
 #  author      hugo benichi
 #  email       hugo.benichi@m4x.org
 #  copyright   2012 hugo benichi  
 #
 ##
 
# Main class and namespace of the program
# which shoud be instancied to use a c_buffer
class CBuffer

  require 'ffi'

  # the API module wraps the native functions of cbuffer.c
  module API
    extend FFI::Library
    lib_type = ENV["OS"] ? "dll" : "so"
    #ffi_lib $LOAD_PATH.map{ |p| p + "/cbuffer/cbuffer.#{lib_type}" }    
    ffi_lib File.dirname(__FILE__) + "/c_buffer/c_buffer.#{lib_type}"
    [
      [ :cbuffer_new,      [:int32, :int32],                :pointer                    ],
      [ :cbuffer_free,     [:pointer],                      :void                       ],
      [ :cbuffer_open,     [:pointer, :pointer],            :int                        ],
      [ :cbuffer_next,     [:pointer],                      :pointer, :blocking => true ],
      [ :cbuffer_has_next, [:pointer],                      :pointer                    ],
      [ :cbuffer_stop,     [:pointer],                      :void                       ],
      [ :cbuffer_write,    [:pointer],                      :int32,   :blocking => true ],
      [ :cbuffer_read,     [:pointer, :pointer, :pointer],  :int32,   :blocking => true ],
    ].each{ |sig| attach_function *sig }
  end
  
  # the CBufferRaw class wraps the native struct memory used to store the buffer memory
  # not used at the momentm here for reference only
  class CBufferRaw < FFI::Struct
    layout  :inbound,   :pointer,
            :outbound,  :pointer, 
            :start,     :pointer,
            :stop,      :pointer,
            :output,    :pointer,
            :chunk,     :int   
  end
  
  # the constructor simply allocates a new FFI::Struct as a local variable
  # and opens a new file at path if path argument provided
  # <TO_DO>  
  def initialize args 
    @chunk   = args[:chunk]  || 1
    @length  = args[:length] || 2
    @path    = args[:path]
    @struct = API::cbuffer_new @chunk, @length
    if @path && @path.is_a?(String) && API::cbuffer_open( @struct, @path ) < 0
      raise IOError, 'error while opening c_buffer output file at %s' % @path
    end
    self
  end

  # provide access to the next raw memory chunk
  # to input data into the buffer
  def input
    API.cbuffer_next @struct
  end
  
  # helper method to start an asynchronous write 
  def run &block
    running = true
    self.define_singleton_method :stop do
      API.cbuffer_stop @struct
      sleep 1 while running
      API.cbuffer_free @struct
    end
    Thread.new do
      #begin
        block.call
        running = false
      #catch
      #  puts "error in writing thread"
      #end
    end      
  end

  # use CBuffer#run to start writing input data to @path
  def output
    self.run { 
      #puts "wrote %i bytes of data" % 
        API.cbuffer_write( @struct)
    }
  end
  
  # use CBuffer#run to start reading all data in the buffer
  # with a user provided block
  def read &block
    self.run { 
      API.cbuffer_read( 
        @struct, 
        @callback = FFI::Function.new( :int, [:pointer, :pointer], :blocking => true ){ |mem,d|
          block.call mem.read_array_of_char @chunk
          0
        }, 
        nil
      )      
    } if block    
  end

end
