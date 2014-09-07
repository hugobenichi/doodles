module Analysis

  module Reader

    class << self

      def parse_settings args
        path = args.select{ |x| x.include?('/') || x.include?('\\') }[0]
        settings = read_settings path
        args.reject!{ |x| x.include? '/' }
        args.map{|s| s.split "=" }.each{ |k,v| settings[k.to_sym] = v.to_i }
        [ settings[:size], 2*settings[:shot], settings[:tomo], path ]
      end

      def read_settings path
        puts "path is","",path,""

        sets = {}
        begin
          File.open( "#{path}/settings", "r") do |file|
            file.readlines.map!{ |com| com.split " " }.each { |com| sets[com[0].to_sym] = com[1] }
          end
          sets.each_key { |k| sets[k] = sets[k].to_i if sets[k].to_i != 0 }
        rescue Exception=> e
          puts " !!"," no settings file -> using default values"," !!"
          sets = {shot: 10000, tomo: 200000, size: 500}
        end
        sets
      end

      def open_data arguments
        frm_args = Reader::parse_settings(arguments)
        Frames.new(frm_args).copy_from( "/shot.dat", "/tomo.dat", "/phase.dat" )
      end

      def open_multi_data arguments
        frm_args  = Reader::parse_settings(arguments)
        data_v    = Frames.new(frm_args).copy_from( "/shot_v.dat", "/tomo_v.dat", "/phase.dat" )
        data_v.extend MultiFrames
        data_v.ax = Frames.new(frm_args).copy_from( "/shot_ax.dat", "/tomo_ax.dat")
        data_v.ap = Frames.new(frm_args).copy_from( "/shot_ap.dat", "/tomo_ap.dat")
        data_v
      end

    end

  end

end
