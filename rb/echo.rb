require "eventmachine"

module Echo_Obj
  include EM::P::ObjectProtocol  

  def unbind
    p "conn closed"
  end

  def post_init
    p "conn started"
  end

  def receive_object msg
    p "received: #{msg}"
    send_object msg
  end

end

module Echo

  def unbind
    p "conn closed"
  end

  def post_init
    p "conn started"
  end

  def receive_data msg
    p "received: #{msg}"
    send_data msg
  end

end

EM::run do 
  EM::start_server( "127.0.0.1", ARGV[0], (ARGV[1] == nil)? Echo : Echo_Obj )
end
