#include dependancies
include Java

require "./ruby/history"

class << javax.swing.SwingUtilities

  class BlockRunner
    def initialize(&proc)
      @p = proc
    end
    def run
      @p.call
    end
  end

  def later(&proc)
    invoke_later BlockRunner.new &proc
  end

end


module LogoJR

  #represents the drawing screen behavior with a canvas backend
  class Screen < javax.swing.JFrame

    import java.awt.Color   #white black red
    import java.awt.BasicStroke
    import java.awt.Dimension
    import java.awt.BorderLayout
    import java.awt.Rectangle

    attr_reader :panel, :geometry

    #represents the accessible color of that screen
    #default color is Color[0]: black
    Colors = [    
      :black,
    ]

    def initialize title, x_width, y_width
      super title

      @geometry = {x: x_width, y: y_width}

      @count = 0
      @panel = javax.swing.JPanel.new
      @panel.background = Color::black
      @panel.set_layout BorderLayout.new

      get_content_pane.add panel, BorderLayout::CENTER
      set_default_close_operation javax.swing.JFrame::EXIT_ON_CLOSE
      set_preferred_size Dimension.new( x_width, y_width)
      set_resizable false
      pack
      set_visible true  
    end

    def paint g
if false
      puts @count += 1
      g.set_paint Color.black
      g.set_stroke BasicStroke.new 2.0

      g.draw Rectangle.new(4,26, @geometry[:x]-2, @geometry[:y]-6)

      puts "traj length", TurtleHistory.trajectories.length

      TurtleHistory.trajectories.each do |line|
        g.draw line
      end

      if false
        g.set_paint Color.red
        g.set_stroke BasicStroke.new 2.0
        g.draw_line 10, 10, 300, 300
        g.draw Line2D::Double.new( 300.0, 10.0, 300.0, 300.0 )
        g.drawString "Hello world !", 10, 50;
      end
end
    end

    class << self

      attr_reader :instance

      def configure x_width, y_width
        javax.swing.SwingUtilities.later do
          @instance = Screen.new "Turtle Graphics Alpha", x_width, y_width
          at_exit {
            @instance.set_visible false
            @instance.dispose
          }
        end
  
        def middle
          return x_width/2, y_width/2
        end

      end
  
      def refresh
        @instance.repaint
      end

    end

  end

end


