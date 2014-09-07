Frames_Dir = File.expand_path(File.dirname(__FILE__)) unless defined? Frames_Dir
require "ffi"

module Frames

  extend FFI::Library
  ffi_lib Frames_Dir + ( (ENV.has_key? "OS")? "/frames.dll" : "/libframes.so" )
  
  [ 
    [:frames_alloc,         [:int,:int,:int,:int],                            :pointer],
    [:frames_free,          [:pointer],                                       :void],
    [:frames_copy,          [:pointer,:int,:pointer,:int,:pointer,:pointer],  :void],
    [:frames_apply_window,  [:pointer,:int,:int,:pointer,:pointer],           :void],
  ].each do |sig| 
    attach_function sig[0], sig[1], sig[2]
  end

  class Session < FFI::Struct
    attr_accessor :buffer    
    layout  :shot_n,    :int,
            :tomo_n,    :int,
            :frm_siz,   :int,
            :byt_siz,   :int,
            :shot,      :pointer,
            :tomo,      :pointer,
            :phase,     :pointer
    def method_missing(sym,*args)
      Frames.send "frames_#{sym}".to_sym, self, *args
    end
  end

  @buffer, @data, @param, @ptrs, @status = nil, nil, {}, {}, 0

  class << self

    def puts msg
      super "Frames >> " + msg
    end
  
    def set_buffer size
      @buffer = FFI::MemoryPointer.new :char, size
    end

    def start_data_transfer conn
      s_n = @param[:shot_num].to_i
      t_n = @param[:tomo_num].to_i
      f_s = @param[:frm_siz].to_i
      @data, @status, @conn = Session.new( frames_alloc(s_n, t_n, f_s, 1) ), 0, conn 
    end

    def data
      @data
    end

    def reset_frames
      @data.free
      @data, @param, @@ptrs = nil, {}, {}
    end

    def stop_data_transfer
      @conn.send_data "close?"
      puts "stopping data transfer"
    end

    def pass_information info
      @param[info[0].to_sym] = info[1]
    end

    def pass_data name, ary
      if name == :phase
        pass_phase_data ary
      else
        pass_quad_data name, ary
      end
    end

    def pass_quad_data n, ary
      @buffer.put_array_of_char(0, ary)
      frames_copy @data[n], @data[:frm_siz], @buffer, ary.length, @ptrs[n][0], @ptrs[n][1]
      p = @ptrs[n][1].get_int(0)
      #puts "#{n} ptr: #{p}" if p % 1000 == 0
    end

    def pass_phase_data phase
      n = :phase
      @data[n].put_array_of_double @ptrs[n], phase
      @ptrs[n] += phase.length
      #puts "#{n} ptr: #{@ptrs[n]}" if @ptrs[n] % 1000 == 0
    end

    def connection_open name
      @status +=1 
      if name == :phase
        @ptrs[name] = 0
      else
        @ptrs[name] = [FFI::MemoryPointer.new(:int), FFI::MemoryPointer.new(:int)]
        @ptrs[name].each { |ptr| ptr.put_int(0,0) }
      end
    end

    def connection_closed name
      puts "tot #{name} frames: #{ name == :phase ? @ptrs[name] : @ptrs[name][1].get_int(0) }"
      @status -= 1
      stop_data_transfer if @status == 0
    end

  end

end
