require 'test/unit'
require 'java'

class FooTest < Test::Unit::TestCase
  def setup
    rnd = Random.new(1234)
    @nums = Array.new(100) { rnd.rand 10000 }
    @foo = Java::Foo.new
    @e = @foo.neutral
  end

  def test_addition_with_neutral
    @nums.each { |i| assert_equal i, @foo.combine(i, @e) }
  end

  def test_addition_with_opposite
    @nums.each { |i| assert_equal @e, @foo.combine(i, @foo.opposite(i)) }
  end

  def test_addition_commutes
    @nums.zip(@nums.shuffle).each { |i,j|
      assert_equal @foo.combine(i,j), @foo.combine(i,j)
    }
  end

end
