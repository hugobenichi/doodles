

module Loop

  require 'ffi'
  extend FFI::Library
  ffi_lib './libloop.so'

  attach_function :loop_scal,   [:int, :pointer, :pointer, :pointer, :pointer], :void


  attach_function :loop_plus,   [:int, :pointer, :pointer, :pointer], :void

  attach_function :scal_plus_0, [:int, :pointer, :pointer],           :double
  attach_function :scal_plus_1, [:int, :pointer, :pointer, :pointer], :void
  attach_function :scal_plus_2, [:int, :pointer, :pointer, :pointer], :void
  attach_function :scal_plus_3, [:int, :pointer, :pointer, :pointer, :pointer], :void
  attach_function :scal_plus_4, [:int, :pointer, :pointer, :pointer, :pointer], :void
  attach_function :scal_plus_5, [:int, :pointer, :pointer, :pointer], :void


  attach_function :loop_apply,  [:int, :pointer, :pointer, :pointer, :pointer], :void
  attach_function :get_plus,    [], :pointer
  attach_function :get_time,    [], :pointer

end

include Loop

n = 4096

def alloc_write ary
  FFI::MemoryPointer.new( :double, ary.length).put_array_of_double(0,ary)
end

FFI::MemoryPointer.class_eval do
  def clear(n)
    self.put_array_of_double( 0, Array.new(n){ 0.0 } ) 
  end
  def randify(n)
    self.put_array_of_double( 0, Array.new(n){ rand } ) 
  end
  def val(n)
    self.get_array_of_double( 0, n)
  end
end

  a, b, d, c =  Array.new(n){rand}, Array.new(n){rand}, Array.new(n){rand}, Array.new(n){0.0}

  double_out = FFI::MemoryPointer.new( :double, 0)

  a_ptr, b_ptr, c_ptr, d_ptr = *[a,b,c,d].map { |x| alloc_write x }
  plus_op = FFI::Function.new(:double, [:double, :double]) { |x,y| x+y }
  time_op = FFI::Function.new(:double, [:double, :double]) { |x,y| x*y } 

if true


  t = Time.new
  100000.times { loop_plus(n, c_ptr, a_ptr, b_ptr) }
  puts "elapsed: #{Time.new-t}"

  c_ptr.clear n

  t = Time.new
  100000.times { loop_plus(n, a_ptr, a_ptr, b_ptr) }
  puts "elapsed: #{Time.new-t}"


  t = Time.new
  #10000.times { loop_apply(n, get_plus, c_ptr, a_ptr, b_ptr) }
  puts "elapsed: #{Time.new-t}"

  c_ptr.clear n

  t = Time.new
  #10000.times { loop_apply(n, plus_op, c_ptr, a_ptr, b_ptr) }
  puts "elapsed: #{Time.new-t}"

end


if false

  t = Time.new
  100000.times { loop_scal(n, c_ptr, a_ptr, b_ptr, d_ptr) }
  puts "elapsed: #{Time.new-t}"

  c_ptr.clear n

  t = Time.new
  100000.times { loop_apply(n, get_plus, c_ptr, a_ptr, b_ptr)
                loop_apply(n, get_time, a_ptr, c_ptr, d_ptr) }
  puts "elapsed: #{Time.new-t}"

  c_ptr.clear n

  t = Time.new
  1000.times { loop_apply(n, plus_op, c_ptr, a_ptr, b_ptr)
                loop_apply(n, time_op, a_ptr, c_ptr, d_ptr) }
  puts "elapsed: #{Time.new-t}"

end


if false

  t = Time.new
  100000.times { scal_plus_0(n, a_ptr, b_ptr) }
  puts "elapsed: #{Time.new-t}"

  t = Time.new
  100000.times { scal_plus_1(n, double_out, a_ptr, b_ptr) }
  puts "elapsed: #{Time.new-t}"

  t = Time.new
  100000.times { scal_plus_2(n, double_out, a_ptr, b_ptr) }
  puts "elapsed: #{Time.new-t}"



  stride = FFI::MemoryPointer.new(:int,3)
  stride.put_array_of_int 0, [1,1,0]

  t = Time.new
  100000.times { scal_plus_3(n, double_out, a_ptr, b_ptr, stride) }
  puts "stride 1,1,0","with pointers", "elapsed: #{Time.new-t}"

  t = Time.new
  100000.times { scal_plus_4(n, double_out, a_ptr, b_ptr, stride) }
  puts "with arrays", "elapsed: #{Time.new-t}"




  stride.put_array_of_int 0, [1,1,1]

  t = Time.new
  100000.times { scal_plus_3(n, c_ptr, a_ptr, b_ptr, stride) }
  puts "1,1,1","with pointers","elapsed: #{Time.new-t}"
  c_ptr.clear n

  t = Time.new
  100000.times { scal_plus_4(n, c_ptr, a_ptr, b_ptr, stride) }
  puts "with arrays","elapsed: #{Time.new-t}"
  c_ptr.clear n

  t = Time.new
  100000.times { scal_plus_5(n, c_ptr, a_ptr, b_ptr) }
  puts "fixed stride","elapsed: #{Time.new-t}"
  c_ptr.clear n


  if false

  m_ptr = FFI::MemoryPointer.new( :double,  n * n)
  m_ptr.put_array_of_double(0, Array.new(n*n){0.0})

  stride.put_array_of_int 0, [1,1,n]

  t = Time.new
  100000.times { scal_plus_3(n, m_ptr, a_ptr, b_ptr, stride) }
  puts "","with pointers","elapsed: #{Time.new-t}"
  m_ptr.clear n*n

  t = Time.new
  100000.times { scal_plus_4(n, m_ptr, a_ptr, b_ptr, stride) }
  puts "with arrays","elapsed: #{Time.new-t}"
  m_ptr.clear n*n


  stride.put_array_of_int 0, [n,1,1]

  t = Time.new
  100000.times { scal_plus_3(n, a_ptr, m_ptr, b_ptr, stride) }
  puts "","with pointers","elapsed: #{Time.new-t}"

  t = Time.new
  100000.times { scal_plus_4(n, a_ptr, m_ptr, b_ptr, stride) }
  puts "with arrays","elapsed: #{Time.new-t}"

  end


end



if false

  a = [0.1,0.2,0.3,0.4]
  b = [1.0,2.0,3.0,4.0]
  c = [0.0,0.0,0.0,0.0]

  a_ptr, b_ptr, c_ptr = [a,b,c].map{|x| alloc_write x}

  loop_apply(4, get_plus, c_ptr, a_ptr, b_ptr)
  loop_apply(4, get_plus, a_ptr, a_ptr, b_ptr)  #attention a l aliasing !

  p c_ptr.val(4), a_ptr.val(4)
  
end

