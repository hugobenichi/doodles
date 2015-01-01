require 'test/unit'
require 'shoulda'
require '%s/lib/sol' % Dir.getwd

include SoL

class VectorClassMethods < Test::Unit::TestCase

  context 'Vector::get_stride methods' do
    should 'get the good stride from shape args' do
      shapes   = [ [1,4], [4,1] , [4,3], [3,3,3]  ]
      strides  = shapes.map{ |x| Vector::get_stride x }
      expected = [ [0,1], [1,0], [3,1], [9,3,1] ]
      strides.each_index do |i|
        assert_equal  strides[i], expected[i]
      end
    end    
  end

  context 'Vector::get_shape methods' do  
    def setup
      @arrays   = [7,12,13,16,20,25].map{ |x| Array.new(x) }
      @target   = [ [1,4], [4,1] , [4,3], [3,3,3]  ]
      @expected = [ [0,1], [1,0], [3,1], [9,3,1] ]
    end
    should 'return the same shape arg if shape is an array' do
      @target.each do |shape| assert_equal shape, Vector::get_shape([],shape) end
    end    
    should 'return the nearest upper power of 1/2 when shape = :sqr | :square' do
      expected = [ [3,3], [4,4], [4,4], [4,4], [5,5], [5,5] ]
      expected.each_index {|i| assert_equal expected[i], Vector::get_shape(@arrays[i], :sqr)}
      expected.each_index {|i| assert_equal expected[i], Vector::get_shape(@arrays[i], :square)}
    end   
    should 'return the nearest upper power of 1/3 when shape = :cub | :cube' do
      expected = [ [2,2,2], [3,3,3], [3,3,3], [3,3,3], [3,3,3], [3,3,3] ]
      expected.each_index {|i| assert_equal expected[i],Vector::get_shape(@arrays[i],:cub)}
      expected.each_index {|i| assert_equal  expected[i], Vector::get_shape(@arrays[i],:cube)}
    end   
    should 'return the most regular rectangle when shape is a fixnum' do
      dim = 2
      expected = [ [3,3], [3,4], [4,4], [4,4], [4,5], [5,5] ]
      expected.each_index {|i| assert_equal expected[i],Vector::get_shape(@arrays[i],2)}
      dim = 3
      expected = [ [2,2,2], [2,2,3], [2,3,3], [2,3,3], [3,3,3], [3,3,3] ]
      expected.each_index {|i| assert_equal expected[i],Vector::get_shape(@arrays[i],3)}
    end    
    should 'complete shape arrays with nil inside' do
      Vector::get_shape( Array.new(16), [nil,2,4] )
    end
  end

end

class VectorCreation < Test::Unit::TestCase

  context 'A Vector made with an Array' do

    def setup
      @x = [0,1,2,3,4,5,6,7]
      @y = [1,1,1,1]
      @z = [1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4]
    end

    should 'not explode in flame' do
      a = @x.vec 
      b = @y.vec 
      c = @y.vec 
      assert a.is_a? Vector
      assert b.is_a? Vector
      assert c.is_a? Vector
    end

    should 'have the default shape and stride of a vector ([1,ary.lentgh], [0,1])' do
      v = @x.vec
      assert_equal v.shape,  [1, @x.length], 'shape'
      assert_equal v.stride, [0,1],          'stride'
    end

    should 'have a settable shape at creation' do
      v = @x.vec as: [2,4]
      w = @z.vec as: [4,4]
      u = @y.vec as: [4,1]
      assert_equal v.shape, [2,4], '[2,4]'
      assert_equal w.shape, [4,4], '[4,4]'
      assert_equal u.shape, [4,1], '[4,1]'
    end

    should 'complete shape args with nil inside' do
      a = @x.vec shape: [nil,2,4]
    end

  end

end

