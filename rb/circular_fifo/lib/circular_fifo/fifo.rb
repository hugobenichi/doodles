module CircularFifo

    class Fifo

        def initialize(size, &populate)
            @depth = next_power size
            populate ||= proc { }
            @blocks = Array.new(@depth) { populate[] }
            @push_at, @pop_at = 0, 0
        end

        def next_power(n, p=1); p >= n ? p : next_power(n, 2*p) end

        def empty?; @push_at == @pop_at end

        def full?; (@push_at - @pop_at) == @depth end

        # invokes a client block on the next slot to write into
        # advances the writing slot by one after the block returns
        # push! will not check if the next slot to write into has been
        # read from yet, therefore beware of state validity !
        def push! &put
            ( put || proc {|obj|} ).call @blocks[ @push_at & (@depth-1) ]
            @push_at += 1
        end

        # invokes a client block on the next slot to read from
        # advances the reading slot by one after the block returns
        # pop! will not check if the next slot to read from has been
        # written into yet, therefore beware of state validity !
        def pop! &use
            ( use || proc {|obj|} ).call @blocks[ @pop_at & (@depth-1) ]
            @pop_at += 1
        end

        # safe call to push! which blocks until the fifo is not full
        def push &put
            #Thread.pass while self.full?
            sleep 0.01 while self.full?
            self.push! &put
        end

        # safe call to pop! which blocks until the fifo contains something
        def pop &use
            #Thread.pass while self.empty?
            sleep 0.01 while self.empty?
            self.pop! &use
        end

    end

end
