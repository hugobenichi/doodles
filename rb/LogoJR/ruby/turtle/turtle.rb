

module LogoJR

  #check if TurtleScreen exist and import if not the case
  require "./ruby/screen" unless defined?( Screen)

  #represents a turtle for graphics
  module Turtle

    require "./ruby/turtle/attributes"
    require "./ruby/turtle/state"
    require "./ruby/turtle/movement"

    attr_reader :history

    All = []

    H = Home = { #assign default values for home
      x:        0.0,
      y:        0.0,
      angle:    90.0,
      pen:      :down,
      color:    Screen::Colors[0]
    }

    def action &proc
      #yield bloc inside itself
    end

    def init
      @history = []   
      class << @history
        def print
          self.each { |state| state.print }
        end
      end   
      home
      self
    end

    class << self

      def create #add arg for changing apparance
        #check if max turtle is not attained
        All << Turtle::State.new 
        All[-1].extend(Turtle::Movement, Turtle).init        
      end

    end

  end

end


