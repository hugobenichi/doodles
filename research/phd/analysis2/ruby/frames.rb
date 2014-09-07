module Analysis

  class Frames < FFI::ManagedStruct

    attr_accessor :buffer, :path
    attr_reader   :phase,  :quad, :shot
    
    layout  :shot_n,    :int,
            :tomo_n,    :int,
            :frm_siz,   :int,
            :shot,      :pointer,
            :tomo,      :pointer,
            :phase,     :pointer

    def self.release ptr
      Analysis::frame_free ptr
    end

    def initialize args
      frm_s, sht_n, frm_n, path = *args
      super frames_alloc(sht_n, frm_n, frm_s)
      self.set_buffer
      @path = path
      self
    end

    def method_missing(sym,*args)
      Analysis.send "frames_#{sym}".to_sym, self, *args
    end

    def set_buffer
      @buffer    = FFI::MemoryPointer.new :char,    self[:frm_siz]
      @shot_mode = FFI::MemoryPointer.new :double,  self[:shot_n]
      @tomo_mode = FFI::MemoryPointer.new :double,  self[:tomo_n]
      @mode_func = FFI::MemoryPointer.new :double,  self[:frm_siz]
    end

    def copy_from( shot, tomo, phase = nil )
      frm_s, sht_n, frm_n, path = self[:frm_siz], self[:shot_n], self[:tomo_n], self.path
      copy_shot sht_n, frm_s, File.open(path + shot,"r")
      puts "shot done ... "
      copy_tomo frm_n, frm_s, File.open(path + tomo,"r")
      puts "tomo done ... "    
      copy_phase File.open(path + phase,"r") if phase
      puts "phase done"
      self
    end

    def copy_shot frames, size, file
      frames.times do |i| 
        @buffer.put_array_of_char 0, file.read(size).unpack( "c#{size}")
        copy_to_shot @buffer, i
      end
    end

    def copy_tomo frames, size, file
      frames.times do |i| 
        @buffer.put_array_of_char 0, file.read(size).unpack( "c#{size}")
        copy_to_tomo @buffer, i
      end
    end

    def copy_phase file
      @phase = file.readlines.map!{ |p| p.to_f}
      @phase.each_with_index { |p,i| self[:phase].put_double(i,p) }
    end

    def variance average = 0
      puts "calculating pt variance ... "
      frm_num, frm_siz = self[:tomo_n], self[:frm_siz]
      ptrs = self[:tomo].get_array_of_pointer 0, frm_num
      var  = FFI::MemoryPointer.new :double, frm_siz

      var.put_array_of_double( 0, Array.new(frm_siz){ 0.0 } )
      if average == 0
        frm_num.times {|i| var_addto( ptrs[i], var, frm_siz) }
      else
        frm_num.times { |i| var_addto_smth( ptrs[i], var, frm_siz, average) }
      end

      var.get_array_of_double(0, frm_siz).map! do |v| v/frm_num end
    end

    def mean
      Analysis::frames_mean(self).get_array_of_double(0, self[:shot_n] + self[:tomo_n] )
    end

    def apply_mode parameters
      @mode_func.put_array_of_double 0, Analysis::Projector::get_mode_function( 
        parameters[:projector],
        self[:frm_siz], 
        parameters[:center], 
        parameters[:gamma], 
        parameters[:kappa],  
        parameters[:alpha],  
      )
      if parameters[:average] == 0  
        apply_window @mode_func, @shot_mode, @tomo_mode
      else
        apply_window_smth @mode_func, @shot_mode, @tomo_mode, parameters[:average]
      end
      @quad = @tomo_mode.get_array_of_double 0, self[:tomo_n] 
    end

    def get_quad parameters
      apply_mode parameters
      shotnoise_norm
      unshift if parameters[:unshift]
      normalize
      @quad
    end

    def shotnoise_norm
      @shot = Math::sqrt(2) * Analysis::var_comp( @shot_mode, self[:shot_n] )
    end

    def normalize
      @quad.map!{ |x| x / @shot }
    end

    def unshift
      m = 0.0
      @quad.each{ |x| m += x }
      m /= quad.length
      puts "shifting quadrature data by:  #{m}",""
      @quad.map!{ |x| x - m }
    end

    def negativity parameters
      puts "calculating negativity ... "
      margi = Quix::Marginal.new Array.new(self[:tomo_n]){ 0.0 }, get_quad(parameters)
      Array.new(5){|i| [5.0+i, margi.negativity(:kirt, 5.0+i)] }
    end

    def process_phase parameters
      case parameters[:phase]
        when :original then Phase.process @phase, parameters[:phase_smoothing]
        when :random   then Array.new(self[:tomo_n]) { Math::PI*rand }        
        when :fitting
      end
    end

    def to_marginal param
      Quix::Marginal.new process_phase(param), get_quad(param), self[:tomo_n]
    end

  end

end
