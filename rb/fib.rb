#!/usr/bin/env ruby

class Fib

  def initialize
   @x, @y = 0, 1
  end

  def next
		y = @y
    @x, @y = @y, @x+@y
    return y
	end

  def restart
   @x, @y = 0,1
  end

end

f = Fib.new

10.times { puts f.next }
