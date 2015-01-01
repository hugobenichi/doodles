#!/usr/local/bin/ruby

class Node
    attr_accessor   :val, :n
    attr_accessor   :left, :right

    def initialize(val = nil); @val = val; @n = 1 end
    def to_s; "%s-(%s,%s)" % [(@val || "nil"), @left, @right].map(&:to_s) end
    def to_s; @val.to_s end

    def verify
        return true if @left == nil && @right == nil
        return verify_right unless @left
        return verify_left unless @right
        return verify_left && verify_right
    end

    def verify_left; @left.val <= @val && @left.verify end

    def verify_right; @val <= @right.val && @right.verify end

    def balance
        balance_left
        balance_right
    end

    def balance_left
        if @left && @left.val > @val
            @left.val, @val = @val, @left.val
            @left.balance
        end
    end

    def balance_right
        if @right && @right.val < @val
            @right.val, @val = @val, @right.val
            @right.balance
        end
    end

    def insert new_val
        @n += 1
        if @left == nil && @right == nil
            if new_val < @val
                @left = Node.new(new_val)
            else
                @right = Node.new(new_val)
            end
            return
        end
        min, max = *[new_val, @val].sort
        unless @left
            @left = Node.new(min)
            @val = max
            balance_right
            return
        end
        unless @right
            @right = Node.new(max)
            @val = min
            balance_left
            return
        end
        if @left.n < @right.n
            @left.insert min
            @val = max
            balance_right
        else
            @right.insert max
            @val = min
            balance_left
        end
    end

    [:left, :right].each do |child|
        define_method("insert_#{child}".to_sym) do |new_val|
            if self.send(child)
                self.send(child).insert new_val
            else
                self.send("#{child}=".to_sym, Node.new(new_val))
            end
        end
    end

    alias_method "<<".to_sym, :insert

    def pr_zigzag
        i = 0
        next_rank = [self]
        until next_rank.empty?
            rank = []
            next_rank.each{ |x| 
                rank << x.right if x.right
                rank << x.left if x.left
            }
            next_rank.reverse! if i % 2 == 1
            next_rank.each{ |x| print "#{x.val}"}
            print "\n"
            next_rank = rank
            i += 1
        end
    end

    def pr
        all_ranks = []
        current = [self]
        while current.inject(false){|all,x| all || x}
            next_rank = []
            current.each do |x|
                if x
                    next_rank << x.left
                    next_rank << x.right
                end
            end
            all_ranks << current
            current = next_rank
        end
        all_ranks.reverse!
        offset = ""
        space  = " "
        all_ranks.map!{ |nodes|
            t = offset + nodes.map{|x| x ? x : " " }.join(space)
            offset = space
            space = space + space + " "
            t
        }
        puts all_ranks.reverse!.join("\n")
    end

end


t = Node.new('h')
['d', 'l', 'b', 'f', 'j', 'n', 'a', 'c', 'e', 'g', 'i', 'k', 'm', 'o'].shuffle.each{|x|

#t = Node.new(9)
#Array.new(9){|i|i}.shuffle.each{|x|
    puts "-> #{x}"
    t.insert x
    t.pr; puts t.verify, ""
}


#t = Node.new('a')
#['b', 'c', 'd', 'e', 'f', 'g'].shuffle.each{|x| puts "-> #{x}"; t.insert x; t.pr; puts ""}


