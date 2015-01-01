include Java


module LogoJR

  #store the history of the turtle's displacements
  module TurtleHistory


    import java.awt.geom.Line2D

    #an array of every turtle's state history
    @turtles = [[]]

    #an array of every turtle's trajectory
    @lines = []  #?? might be a perforamce killer when lots of drawing

    class << self

      #access a turtle history
      def [](turtle)
        #unless @turtles.has_key? turtle
        #  @turtles[turtle] = []
        #end
        @turtles[0]
      end

      #add a state to a turtle history and parse the information for drawing
      #if the trajectories changes redraw the screen
      def add turtle, state  
        @turtles[0] << state
        if @turtles[0].length > 1        
          @turtles[0][-2].print
  
          x0, y0 = @turtles[0][-2][:x], @turtles[0][-2][:y]
          x1, y1 = state.x, state.y        
          @lines << Line2D::Double.new( x0, y0, x1, y1 )
    
          Screen::refresh

        end
        
        #if pen down add to line    
      end

      #pop the turtle last state and process it out of the drawline pipeline
      def pop turtle
        state = @turtle[0].pop
        @lines.pop
        #remove this state last pt
        state
      end

      #output the trajectories of every turtle
      def trajectories
        @lines
      end

    end

  end

  #alias for TurtleHistory
  TH = TurtleHistory

end


