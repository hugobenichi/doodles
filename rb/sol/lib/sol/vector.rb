

module SoL

  class Vector

    class << self
      def get_shape(ary,shape=nil)
        case shape
        when Array then complete_shape( shape, ary.size)
        when Symbol   
          exp = {sqr:2,square:2,cub:3,cube:3}[shape] || 2
          Array.new(exp) { ary.size.**(1.0/exp).ceil }
        when Fixnum   
          Array.new(shape) { ary.size.**(1.0/shape).to_i }.tap { |rez|
            i = -1           
            rez[-1-(i+=1)] += 1 while rez.inject(1.0){|s,x| s *= x} < ary.size 
          }
        else [1, ary.size]
        end
      end
# PARTIAL IMPLEM
      def complete_shape( shape, size)
        return shape unless shape.include? nil
        return shape.map{|x| x.nil? ? 1 : x}
      end
      def get_stride(shape)
        inner = 1
        [].tap{ |stride| 
          shape.reverse_each do |x| 
            stride << (x == 1 ? 0 : inner) #!careful with precedence 
            inner *= x 
          end
        }.reverse!
      end
    end

    attr_accessor :shape
    attr_reader   :stride, :data, :type, :mem, :size, :hook

    def initialize(args={})
      @type     = args[:type] || args[:typ] || :double
      @data     = args[:data] || [] #nil
      @shape    = Vector::get_shape( @data, args[:as] || args[:dim] || args[:shape] )
      @stride   = Vector::get_stride @shape
      @size     = @shape.inject(1){|len,dim| len *= dim}
      @mem      = FFI::MemoryPointer.new( @type, @size )
      @hook     = SoL.const_get @type.to_s.capitalize.to_sym
      if block_given?
# PARTIAL IMPLEM

      end
    end

    def cpy(args=nil)  #return same type of array
      if args[:from]
        #copy data with coercion
        self
      elsif args[:to]
        args[:to].cpy( {from: self}.merge! args ) 
      else
# PARTIAL IMPLEM: take into account the reshape case
        Vector.new( as: @shape, type: @type, data: @data )
      end
    end

    def !@
      Vector.new( as: @shape, type: @type, data: @data )
    end

  end

end

