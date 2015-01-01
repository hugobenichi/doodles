require '%s/lib/sol' % Dir.getwd


len = 10
a, b, c = SoL::Vector.new(as: [1,len]), 
          SoL::Vector.new(as: [1,len]), 
          SoL::Vector.new(as: [1,len])


unless RUBY_ENGINE == 'rbx'                                       # MRI, JRuby

  [c, a, b ].each_with_index do |vec,i| 
    vec.mem.put_array_of_double( 0, Array.new(len){ |j| 1.0*i*j } )                      
  end

  p a.mem.get_array_of_double(0,len)   

  a.sqr + b

  [a,b,c].each do |x| p x.mem.get_array_of_double(0,len) end    

else                                                              # Rubinius

  [c, a, b ].each_with_index do |vec,i| 
    vec.mem.write_array_of_type( :double, :write_double, Array.new(len){ |j| 1.0*i*j } )
  end
  #SoL::Double::sol_double_linear2_plus(a,b,c,len)
  SoL::Double::linear2_plus(a,b,c,len)
  [a,b,c].each do |x| p x.mem.read_array_of_type( :double, :read_double, len) end


end



