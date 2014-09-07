require "rubygems" #necessary for JR14 but not in R19
require "eventmachine"

module Data_Client

  def self.start ip, port, settings, &b
    define_method(:ip)        { ip }
    define_method(:port)      { port }
    define_method(:settings)  { settings }
    define_method(:callback, b)
    EM::run do 
      EM.set_timer_quantum 5
      EM::connect( ip, port, Data_Client)
    end
  end
  
  def post_init
    puts @msg ="","Connection to data server established"  
  end

  def unbind
    puts "", "Connection to data server closed"
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
      when "data?" 
        create_senders
        callback self
      when "close?"
        close_connection_after_writing
    end
    @msg = ""
  end

  def information
    [
      "information;path;#{settings[:path]}!",
      "information;frm_siz;#{settings[:size]}!",
      "information;shot_num;#{2*settings[:frame]*settings[:shot]}!",
      "information;tomo_num;#{settings[:frame]*settings[:tomo]}!",
    ].each { |info| send_data info }
    send_data "start!"
  end

  def create_senders
    @senders = {}
    @senders[:shot] = EM::connect ip, port+1, Sender::Shot
    @senders[:tomo] = EM::connect ip, port+2, Sender::Quad
    @senders[:phase]= EM::connect ip, port+3, Sender::Phase
  end

  def [](name)
    @senders[name]
  end
  
  def each
    @senders.each_value do |conn| yield conn end
  end
  
  module Sender
    include EM::Deferrable
    def post_init
      puts "#{name} data sender conn open"  
      @tot_sent = 0
      callback do close_connection_after_writing end
    end
    def unbind
      puts "#{name} data sender conn closed, tot sent: #{@tot_sent}" 
    end
    def send_frame msg
      @tot_sent += msg.bytesize
      send_data msg
    end
    
    module Shot
      include Sender
      def name; "shot" end
    end
    module Quad
      include Sender
      def name; "tomo" end
    end
    module Phase
      include Sender
      def name; "phase" end
    end

  end

end
