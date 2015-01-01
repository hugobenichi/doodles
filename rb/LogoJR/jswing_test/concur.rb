#require 'java'
include Java

module Drawing

  import javax.swing.JFrame
  import javax.swing.JPanel
  import javax.swing.JButton
  import javax.swing.JLabel
  import javax.swing.UIManager
  import javax.swing.SwingUtilities

  import java.awt.Color   #white black red
  import java.awt.BasicStroke
  import java.awt.Dimension
  import java.awt.BorderLayout
  import java.awt.Graphics
  import java.awt.Graphics2D
  import java.awt.Rectangle
  import java.awt.geom.Line2D


  class Tos

    attr_reader :panel

    def initialize

      @panel  = JPanel.new
      @window = JFrame.new "Turtle Graphics Alpha"

      class << @panel
        attr_accessor :g
        def init
          @g = self.graphics
          @g.set_paint Color.white
          @g.set_stroke BasicStroke.new 1.0
          def init; end
        end
        def paint1
          @g.draw Rectangle.new(10,10,100,100)
          @g.draw_line 10, 10, 300, 300
          @g.drawString "Hello world !", 10, 50;
        end
        def paint2
          @g.draw Rectangle.new(20,20,100,100)
          @g.draw Rectangle.new(100,20,100,100)
          @g.draw Line2D::Double.new( 300.0, 10.0, 300.0, 300.0 )
        end
      end

      SwingUtilities.invoke_later self

    end

    def run

      @panel.background = Color::black
      @panel.set_layout BorderLayout.new

      @window.get_content_pane.add @panel, BorderLayout::CENTER
      @window.set_default_close_operation JFrame::EXIT_ON_CLOSE
      @window.set_preferred_size Dimension.new(440, 440)
      @window.set_resizable false
      @window.pack
      @window.set_visible true
      @window.repaint      

      @panel.init 

    end

    def stop
      @window.set_visible false
      @window.dispose
    end

    @count = 0

    def paint
      puts @count += 1
    end

  end

end

t = Drawing::Tos.new

if true
  sleep 0.5
  t.panel.paint1
  sleep 0.5
  t.panel.paint2
  sleep 0.5
STDIN::gets
  t.stop
end

#STDIN::gets
#t.stop

