  require 'ffi'
 
    # Use FFI to stuff our bytecode somewhere on the heap.here's the malloc(3)/memcpy(3) combo
    memp = FFI::MemoryPointer.from_string STDIN.read
 

    funcptr = FFI::Function.new( FFI.find_type(ret), args, memp, :convention => :default )

    # Now we call our bytecode stub directly.
    # This is basically like saying "funcptr();" in the C version.
    funcptr.call()


