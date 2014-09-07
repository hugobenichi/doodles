require "rubygems" 
require "eventmachine"

module Data_Server

  def puts msg
    super "Data Server >> " + msg
  end

  def post_init
    puts "new connection from data client"
    @time, @msg = Time.new, ""
    send_data "information?"
  end

  def unbind
    puts "connection stopped  / time: #{(Time.new-@time).truncate} sec"
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
        Frames.start_data_transfer self
        send_data "data?"
    end
    @msg = ""
  end

  module Receiver

    def puts msg
      super "Receiver #{name} >> " + msg
    end

    def post_init
      puts "connection with data sender started"
      @call, @siz, @max = 0, 0, 0
      Frames.connection_open name
      @writer = Writer.new.write name, "./data/#{name}" #path 
      @timer = EM::add_periodic_timer(1) do
        puts "current  call #{@call}  siz #{@siz}"
      end
    end

    def unbind
      @timer.cancel unless @timer == nil
      puts "connection with data sender closed"
      puts "tot receive call: #{@call}  tot size: #{@siz}  max msg: #{@max}"
      Frames.connection_closed name
      @writer.succeed
    end

    def receive_data msg
      @call += 1
      @siz += msg.bytesize
      @max = msg.bytesize if msg.bytesize > @max
      Frames.pass_data name, msg.unpack( encode(msg.bytesize))
      @writer.buffer.push msg
    end

    def encode l
      "c#{l}"
    end

    module Shot  
      include Receiver
      def name; :shot end
    end

    module Tomo
      include Receiver
      def name; :tomo end
    end

    module Phase
      include Receiver
      def name; :phase end
      def encode l
        "e#{l/4}"
      end
    end

  end

end

require "frames"
require "writer"
#require "../tomography/tomography"
#require "../quix/ruby/quix"
#require "../irb_embedded"

EM::run do

  Frames.set_buffer 50000

  ip, port = ARGV[0], ARGV[1].to_i
    
  EM.set_timer_quantum 5
  EM::start_server ip, port+0, Data_Server
  EM::start_server ip, port+1, Data_Server::Receiver::Shot
  EM::start_server ip, port+2, Data_Server::Receiver::Tomo
	EM::start_server ip, port+3, Data_Server::Receiver::Phase

  #irb_t = Thread.new { IRB.start_session binding }

end

