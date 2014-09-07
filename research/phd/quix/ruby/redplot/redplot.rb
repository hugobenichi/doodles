#  RedPlot is Copyright (c) 2011 Hugo Benichi <hbenichi@gmail.com>
#
#  RedPlot is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  RedPlot is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
#  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#  See the GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along with RedPlot.
#  If not, see <http://www.gnu.org/licenses/>.
#
# Description:  a simple layer to gnuplot;
#               can enhance any ruby object with plotting facilities;
#
# How it works: cf examples/; 
#               you can either mixin Redplot to any object; 
#               you can also directly use the Plotter class;
# 


#main namespace
#also used for mixin
module RedPlot
  
  VersionNumber, VersionDate = '0.1.2', '2011-04-04'

  #helper class to store callbacks to the data and plot command options
  class ToDraw
    attr_reader :options, :callbacks 
    def initialize
      reinit
    end      
    def reinit
      @options, @callbacks = [], []
      self
    end
    #main client method
    #expects a callback block that return an object with access to data to plat
    #different object are possible (cf Plotter#formatdata)
    def add(option="", &block)
      unless block.nil?
        @options    << option
        @callbacks  << block
      end
      self
    end
    alias_method :clear, :reinit #add to test
  end
  
  #main class whose instances are wrapper to gnuplot process
  #the instances manage the necessary ressources and have all the callable methods
  #
  #if an object is mixed with RedPlot, then an instance of Plotter is hooked to that object
  #the instance of Plotter is then the path to gnuplot and helps to avoid name conflicts
  class Plotter  

    attr_accessor :header,      # stores in an array settings such as "set xrange [a,b]" 
                  :path,        # gives a path to save data and pritn graph
                  :command      # determine the main plotting command: plot, splot, ...
    attr_reader   :todraw       # holds callbacks to the data to be plotted

    #destructor to close the gnuplot process
    def self.release instance
      instance.close
    end

    def method_missing(*args)
      (scoped_header = @header ) << args.join(" ") + " "
      args[0].to_s.tap do |eval_space|        
        eval_space.singleton_class.instance_eval do 
          define_method(:method_missing) do |*args| 
            scoped_header[-1] << args.join(" ")  + " "
            self
          end
        end
      end
    end

    def initialize(args={}, &configuration_block)
      @header   = args[:header]   ||  []
      @path     = args[:path]     ||  'redplot'
      @command  = args[:command]  ||  'plot'                
      @todraw   =                     ToDraw.new
      self.instance_eval &configuration_block if block_given?
      self
    end

    #start the gnuplot process if @proc is nil
    def start
      @proc ||= IO.popen("gnuplot","w+").tap do |open|
        ObjectSpace.define_finalizer(self) { open.close }
      end
    end

    #directly write to the gnuplot process stdin
    def <<( *args )
      start.puts args
    end

    #main client method to draw data
    #happens in 3 steps:  
    #  1) sends the header args to gnuplot
    #  2) sends the plot command
    #  3) sends the data one block at a time   
    def draw
      start
      @proc.puts @header
      @proc.puts format_command( @command, @todraw.options )
      @todraw.callbacks.each { |block|  @proc.puts format_data( block.call ) }
      self
    end

    #format the plot command 
    def format_command( command, options )               
      "%s '-' %s" %  [command, options.join(",'-' ")] #not tested yet
    end

    #format the data from the callback blocks to fit gnuplot
    #several interface are possible
    #   1) a plain Array
    #   2) an array of arrays, in which case these arrays are taken as data columns
    #   3) anything with an each method 
    #      the block send to each takes one or more Numerics and/or numbers hidden in string 
    def format_data( raw_data )
      if raw_data.is_a? Array 
        if raw_data[0].is_a? Array  
          raw_data.transpose.map!{ |one_row| one_row.join " "}
        else
          raw_data
        end     
      else
       [].tap{ |data| raw_data.each { |*vals| data << vals.join(" ") } } #not tested yet
      end.<< 'end'
    end

    #write to disc the result of format_Data
    def save_data(path=@path)
      File.open( path+".dat", "w+") do |file|
        @todraw.callbacks.each do |block|  
          file.puts format_data( block.call ).tap{|ary| ary[-1] = ''}
        end
      end
      self
    end

    #write to disc the content of @header and format_command
    def save_script(path=@path)
      File.open( path + ".scr", "w+") do |file|
        file.puts @header
        file.puts format_command( @command, @todraw.options )
      end
      self
    end

    #either draw in png or eps mode
    def save_graph(type=:png, path=@path)
      case type
      when :png then save_png( path )
      when :eps then save_eps( path )
      end
    end
  
    #set gnuplot terminal to eps and draw
    #!the terminal will stay in eps mode
    def save_eps(path=@path)
      start.puts  "set term postscript eps color blacktext \"Helvetica\" 24",  
                  "set output '#{path}.eps'"
      draw
    end

    #set gnuplot terminal to png and draw
    #!the terminal will stay in png mode
    def save_png(path=@path)
      start.puts  'set term png enhanced',  
                  "set output '#{path}.png'"
      draw
    end

  end

  #all the mechanism for object extention happens here
  class << self

    #if a class includes RedPlot, RedPlot inserts itself in the class instead
    def included who
      insert in: who, as: :plot
    end

    #if an object extends RedPlot, RedPlot adds itself to the object via its eigenclass
    def extended who
      insert in: who.singleton_class, as: :plot
    end

    #this method takes a class and adds an attr_reader to it
    #it also adds a lazy initialization reader with a shortcut name
    # ! a call to the attr_reader before a call to the shorcut reader will return nil
    def insert(args, &block)
      target = args[:in]  
      name   = args[:as] || :plot
      target.class_eval do 
        attr_reader :gnuplot_wrapper
        define_method(name) { @gnuplot_wrapper ||= RedPlot::Plotter.new(args,&block) }
      end if target.is_a? Class
    end  

    #returns a string with version number
    def version
      'RedPlot version "%s" (%s)' % [RedPlot::VersionNumber, RedPlot::VersionDate]
    end

  end

end


class Array
  #this provide a quick hook for Array instances
  def plot!(args={})
    args[:as] ||= :plot
    args[:in]   = self.singleton_class
    data = self
    RedPlot::insert(args) { todraw.add args[:style]  {data} }
    wrapper = self.send args[:as]
  end
  #hook RedPlot to the array and draw what is inside it
  def plot_now!(args={})
    self.plot!(args).draw
  end
end

 
