#!/usr/local/bin/ruby

require "ffi"

module A
    extend FFI::Library
    ffi_lib "./hw.so"
    attach_function :hw, [:int], :int
end

puts A::hw 10
