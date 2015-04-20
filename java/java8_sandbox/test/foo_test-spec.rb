require 'test/spec'
require 'java'

describe "a Foo" do

  before do
    rnd = Random.new(1234)
    @nums = Array.new(100) { rnd.rand 10000 }
    @foo = Java::Foo.new
    @e = @foo.neutral
  end

  it "should add a number to its neutral element and return the number" do
    @nums.each { |i| @foo.combine(i, @e).should.equal i }
  end

  it "should add a number with its opposite and return the neutral element" do
    @nums.each { |i| @foo.combine(i, @foo.opposite(i)).should.equal @e }
  end

  it "should add two numbers together with commutativity" do
    @nums.zip(@nums.shuffle).each { |i,j|
      @foo.combine(i,j).should.equal @foo.combine(j,i)
    }
  end

end
