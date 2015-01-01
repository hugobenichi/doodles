require '%s/lib/sol/autowrapper' % Dir.getwd

len = 16

a, b, c = Array.new(3).map{ FFI::MemoryPointer.new( :double, len) }


unless RUBY_ENGINE == 'rbx'    # MRI, JRuby

  [c, a, b ].each_with_index do |ptr,i| 
    ptr.put_array_of_double( 0, Array.new(len){ |j| 1.0*i*j } )                      
  end
  #SoL::Double::sol_double_linear2_plus(a,b,c,len)  
  SoL::Double::linear2_plus(a,b,c,len)  
  [a,b,c].each do |x| p x.get_array_of_double(0,len) end    

else  # Rubinius

  [c, a, b ].each_with_index do |ptr,i| 
    ptr.write_array_of_type( :double, :write_double, Array.new(len){ |j| 1.0*i*j } )
  end
  #SoL::Double::sol_double_linear2_plus(a,b,c,len)
  SoL::Double::linear2_plus(a,b,c,len)
  [a,b,c].each do |x| p x.read_array_of_type( :double, :read_double, len) end


end



