
module A

  def foo
    puts "foo"
  end

end

module C

  def foo_with_alias
    foo
  end

  alias_method :bar, :foo_with_alias

end


module B

  extend A
  
  #foo

  #alias_method :bar, :foo

end


B.foo


B.extend C

B.bar


