require "rubygems" 
require "eventmachine"

module Data_Server

  def post_init
    puts "","new connection from data client"
    @time, @msg = Time.new, ""
    send_data "information?"
  end

  def unbind
    puts "", "connection stopped", "elapsed time: #{(Time.new-@time).truncate}"
  end

  def send_data msg
    puts "sending "+msg
    super msg
  end
  
  def receive_data msg
    msg.each_char do |c|
      @msg << c
      parse if c == "!"
    end
  end

  def parse 
    puts "parsed " + @msg
    tokens = @msg[0..-2].split ";"
    case tokens[0]
      when "information" 
        Frames.pass_information tokens [1..-1]
      when "start" 
        Frames.start_data_transfer
        send_data "data?"
    end
    @msg = ""
  end

end

module Receiver

  def post_init
    puts "", "#{name}_receiver : connection with data sender started"
    @total_call, @total_siz, @max = 0, 0, 0
    #@timer = EM::PeriodicTimer.new(0.7) { puts "received data: #{@total_siz}" }
    Frames.connection_open name
  end

  def unbind
    @timer.cancel unless @timer == nil
    puts "#{name}_receiver : connection with data sender closed"
    puts "total call to receive_#{name}: #{@total_call}"
    puts "total size: #{@total_siz}  max msg size: #{@max}"
    Frames.connection_closed name
  end

  def receive_data msg
    l = msg.bytesize
    @total_call += 1
    @total_siz += l
    @max = l if l > @max
    pass_data msg
  end

end
 
module Shot_Receiver  
  include Receiver
  def name
    :shotnoise
  end
  def pass_data msg
    Frames.pass_shot_data msg.unpack "c#{msg.bytesize}"
  end
end

module Tomo_Receiver  
  include Receiver
  def name
    :tomo
  end
  def pass_data msg
    Frames.pass_tomo_data msg.unpack "c#{msg.bytesize}"
  end
end

module Phase_Receiver
  include Receiver
  def name
    :phase
  end
  def pass_data msg
    Frames.pass_phase_data msg.unpack "e#{msg.bytesize/4}"
  end
end

require "frames"
require "../irb_embedded"

EM::run do

  Frames.set_buffer 50000

  EM.set_timer_quantum 5
  EM::start_server ARGV[0], ARGV[1].to_i+0, Data_Server
  EM::start_server ARGV[0], ARGV[1].to_i+1, Shot_Receiver
  EM::start_server ARGV[0], ARGV[1].to_i+2, Tomo_Receiver
	EM::start_server ARGV[0], ARGV[1].to_i+3, Phase_Receiver

  #irb_t = Thread.new do IRB.start_session binding end

end

