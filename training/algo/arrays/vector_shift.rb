#!/usr/local/bin/ruby

# juggle implementation
class Array
    def push_in index, new_val
        index  = index % self.length
        old_val, self[index] = self[index], new_val
        return index, old_val
    end
    def shift_jug k
        return self if k > self.length
        self.dup.tap{ |shifted_array|
            k.gcd(self.length).times do |i|
                index, value = i, shifted_array[i]
                loop do
                    index, value = shifted_array.push_in index + k, value
                    break if index == i
                end
            end
        }
    end
    
end

# reverse implementation
class Array
    def rev! i = 0, j = self.length-1
        while i < j
            self[i], self[j] = self[j], self[i]
            i, j = i+1, j-1
        end
        self
    end
    def shift_rev k
        return self if k >= self.length
        self.dup.rev!.rev!(0, k-1).rev!(k, self.length-1)
    end
    def shift_swap k; self end
end

shft = (ARGV[0] || "3" ).to_i
size = (ARGV[1] || "10").to_i

Array.new(size){|i|i}.tap{|x| 
    print "original "; p x
    print "juggle   "; p x.shift_jug shft
    print "reverse  "; p x.shift_rev shft
    print "swapping "; p x.shift_swap shft
}



