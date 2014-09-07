#!/usr/local/bin/ruby

require "ffi"

module Py
    extend FFI::Library
    ffi_lib "./libpycode.so"
    attach_function :run_code, [:int], :int
end


puts Py::run_code(10)
