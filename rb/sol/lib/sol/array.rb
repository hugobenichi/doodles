
class Array

  def vec(args={})
    SoL::Vector.new( {data: self}.merge! args )
  end

  alias_method :to_vec, :vec

end

