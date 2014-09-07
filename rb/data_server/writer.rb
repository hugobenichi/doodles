require "eventmachine" 

  class Writer 
    include EM::Deferrable
    attr_accessor :buffer

    def write name, path
      puts "Writer #{name} >> started"
      file, @stop, @buffer = File.open(path, "wb"), false, []

      define_singleton_method :partial_write do
        begin
          file.print buffer.shift until buffer.empty?
          EM::next_tick { partial_write } unless @stop
        rescue Exception => e  
          puts "error in Writer #{name} \n#{e}"
        end           
      end

      callback do  
        @stop = true
        partial_write
        file.close
        @timer.cancel if @timer
        define_singleton_method :done do end
      end

      EM::defer { partial_write }
      self

    end

  end
