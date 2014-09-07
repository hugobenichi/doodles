module CircularFifo

    # This class provides a convenience wrapper around FIFO for the case
    # when clients wants to buffer data in native memory (using the FFI
    # interface). MemoryFifo also provide a static function for typical
    # asynchronous writes to disk function using a MemoryFifo instance
    # underneath.
    class MemoryFifo < Fifo

        attr_accessor :bytes,   # size in bytes of memory buffers in fifo
                      :closed   # boolean flag, true if nothing left to push

        def initialize(size, block_bytes)
            super(size) { FFI::MemoryPointer.new(:char, block_bytes) }
            @bytes = block_bytes
            @closed = false
        end

        def close; @closed = true end

        # creates a Thread to push data inside the fifo
        # used internally par MemoryFifo::async_write
        def input &caller
            Thread.new do caller[self]; self.close end
        end

        # creates a Thread to pop data the fifo and writes it to disk
        # used internally par MemoryFifo::async_write
        def output(file_name, mode="a")
            Thread.new do
                IO.raw_file(file_name, mode) do |writer|
                    until self.closed && self.empty?
                        sleep 0.01 if self.empty? # pass execution if empty
                        self.pop{ |mem| writer[mem, @bytes] } until self.empty?
                    end
                end
            end
        end

        def self.async_write(depth, bytes, name, mode='a', &caller)
            MemoryFifo.new(depth, bytes).tap do |fifo|
                [ fifo.output(name, mode), fifo.input(&caller) ].map &:join
            end
        end

    end

end
