module CircularFifo

    # Just a simple writer around libc fwrite / fopen / fclose
    module IO

        extend FFI::Library
        ffi_lib FFI::Library::LIBC

        [
            [:fwrite, [:buffer_in, :size_t, :size_t, :pointer], :size_t],
            [:fopen,  [:string, :string], :pointer, :blocking => true],
            [:fclose, [:pointer], :int]
        ].each{|sig| attach_function *sig}

        def self.raw_file(file_name, mode, &caller)
            begin
                IO.fopen(file_name, mode).tap{ |file|
                    caller[proc{ |mem, bytes|
                        IO.fwrite(mem, 1, bytes, file)
                    }]
                    #caller[writer]
                    IO.fclose(file)
                }
            rescue Exception => orz
                puts "error in CircularFifo::MemoryFifo#output:", orz
            end
        end


    end

end
