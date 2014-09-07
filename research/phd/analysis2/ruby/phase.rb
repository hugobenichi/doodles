module Analysis

  module Phase

    class << self

      def process raw, smooth = 1
        smth = []
        divide_phase(raw).map!{ |f| smooth_phase(f, smooth )}.each { |y| smth += y } 
        norm_phase smth
      end

      def divide_phase phase
        puts "dividing frames ... "
        phase_frames = [[]]
        a = phase[0]
        phase.each do |p|
          phase_frames << [] if (p-a).abs > 3.0
          phase_frames[-1] << p
          a = p
        end
        phase_frames
      end

      def smooth_phase phs_frm, avr
        puts "smoothing phase frm ... "
        buffer = Array.new(avr).map! { phs_frm[0] }
        smth = []
        sum = avr*phs_frm[0]
        phs_frm.each do |p|
         sum += 1.0*p
         buffer.push p
         sum -= 1.0*buffer.shift
         smth << sum / avr
        end
        offset = avr/2
        offset.times{ smth << smth[-1] }
        smth[offset..-1]
      end

      def norm_phase phase
        puts "removing phase offset ... \n"
        a, b = phase.min, phase.max
        m = (b+a)/2
        phase.map!{|p| (p-m)}

        puts "normalizing phase ... \n"
        n = [phase.min.abs, phase.max].max
        phase.map!{|p| p/n}

        puts "mapping acos ... "
        buf_phase = FFI::MemoryPointer.new :double, phase.length
        buf_phase.put_array_of_double 0, phase
        Analysis::phase_acos buf_phase, phase.length

        buf_phase.get_array_of_double 0, phase.length
      end

    end

  end

end
