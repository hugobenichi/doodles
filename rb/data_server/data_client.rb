require "rubygems" #necessary for JR14 but not in R19
require "eventmachine"

Frame = 1000
Frame_num = ARGV[2].to_i #10
Frame_shot_rep = ARGV[3].to_i #10
Frame_tomo_rep = ARGV[4].to_i #10

module Data_Client

  def post_init
    @time, @msg = Time.new, ""
    puts "","Connection to data server established"  
  end

  def unbind
    puts "", "Connection to data server closed", "elapsed time: #{(Time.new-@time).truncate}"
    EM::next_tick { EM::stop_event_loop }
  end

  def receive_data msg
    msg.each_char do |c|
      @msg << c
      parse if c == "?"
    end
  end

  def parse
    puts "received msg #{@msg}"
    case @msg
      when "information?" then information
      when "data?"        then schedule_data
      when "close?"      then close_connection_after_writing
    end
    @msg = ""
  end

  def information
    [
      "information;path;../tos/!",
      "information;frm_siz;#{Frame}!",
      #"information;shot_num;#{2*Frame_num*Frame_shot_rep}!",
      "information;shot_num;#{Frame_num*Frame_shot_rep}!",
      "information;tomo_num;#{Frame_num*Frame_tomo_rep}!",
    ].each { |info| send_data info }
    send_data "start!"
  end

  def send_data msg
    puts "sending "+msg
    super msg
  end

  def schedule_data
    q = EM::Queue.new
    add_data_sender q, ARGV[1].to_i+1, Shot_Sender
    add_data_sender q, ARGV[1].to_i+2, Tomo_Sender
    #add_data_sender q, ARGV[1].to_i+1, Shot_Sender
    add_data_sender q, ARGV[1].to_i+3, Phase_Sender
    #q.push Proc.new {
    #  puts "","finished sending data"
    #  close_connection_after_writing
    #}
    q.pop{ |blk| blk.call }
  end

  def add_data_sender q, port, mixin
    puts "adding data sender on port #{port}"
    q.push Proc.new {
      sender = EM::connect( ARGV[0], port, mixin)
      sender.q = q
      sender.send_frames
    } 
  end

end

module Sender

  attr_accessor :q

  def post_init
    puts "","#{name} data sender connection established"  
    @current_msg, @tot_sent = "", 0
  end

  def unbind
    puts "","#{name} data sender connection closed", "total sent: #{@tot_sent}" 
    @q.pop{ |blk| blk.call }
  end

  def send_frames
    puts "","#{name} setting data timer: #{rep}"
    this_timer = EM::PeriodicTimer.new 0.1 do
      send_frame_packet
      if @tot_sent == rep
        this_timer.cancel
        close_connection_after_writing 
      end
    end
  end

end

module Shot_Sender
  include Sender 
  def name
    :shotnoise
  end
  def rep
    Frame_shot_rep
  end
  def send_frame_packet 
    puts "Shot: sending frame packet ##{@tot_sent}"
    Frame_num.times do send_data Frame_Data.pack "c#{Frame}" end
    @tot_sent += 1
  end
  Frame_Data = Array.new(Frame) do rand 255 end
end

module Tomo_Sender 
  include Sender
  def name
    :tomo
  end
  def rep
    Frame_tomo_rep
  end
  def send_frame_packet 
    puts "Tomo: sending one frame packet ##{@tot_sent}"
    Frame_num.times do send_data Frame_Data.pack "c#{Frame}" end
    @tot_sent += 1
  end
  Frame_Data = Array.new(Frame) do rand 255 end
end

module Phase_Sender
  include Sender
  def name
    :phase
  end
  def rep
    Frame_tomo_rep
  end
  def send_frame_packet 
    puts "Phase: sending one phase packet ##{@tot_sent}"
    Frame_num.times do send_data [rand].pack "e1" end
    @tot_sent += 1
  end
end

EM::run do

  EM.set_timer_quantum 5
  EM::connect( ARGV[0], ARGV[1].to_i, Data_Client)

end

puts "event loop stopped"

