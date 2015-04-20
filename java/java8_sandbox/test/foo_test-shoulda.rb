require 'shoulda'
require 'java'

class FooTest < Test::Unit::TestCase

  def setup
    rnd = Random.new(1234)
    @nums = Array.new(100) { rnd.rand 10000 }
    @foo = Java::Foo.new
    @e = @foo.neutral
  end

  should "add a number to its neutral element and return the number" do
    @nums.each { |i| assert_equal i, @foo.combine(i, @e) }
  end

  should "add a number with its opposite and return the neutral element" do
    @nums.each { |i| assert_equal @e, @foo.combine(i, @foo.opposite(i)) }
  end

  should "add two numbers together with commutativity" do
    @nums.zip(@nums.shuffle).each { |i,j|
      assert_equal @foo.combine(i,j), @foo.combine(j,i)
    }
  end

end
