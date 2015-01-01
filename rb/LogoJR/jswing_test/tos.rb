#require 'java'
include Java

module Drawing

  import javax.swing.JFrame
  import javax.swing.JPanel
  import javax.swing.JButton
  import javax.swing.JLabel
  import javax.swing.UIManager

  import java.awt.Color
  import java.awt.Dimension
  import java.awt.BorderLayout
  import java.awt.Graphics
  import java.awt.Graphics2D
  import java.awt.Rectangle

  import java.awt.geom.Line2D

  panel = JPanel.new
  panel.background = Color::white
  panel.set_layout BorderLayout.new

  #g = panel.graphics

  class << panel

    attr_accessor :g

    @g = nil

    def auto_init

      if @g == nil

        @g = self.graphics

        class << @g

          def paint1

            draw Rectangle.new(10,10,100,100)
            draw_line 10, 10, 300, 300
            drawString "Hello world !", 10, 50;

          end

          def paint2

            draw Rectangle.new(20,20,100,100)
            draw Rectangle.new(100,20,100,100)
            draw Line2D::Double.new( 300.0, 10.0, 300.0, 300.0 )

          end

        end

        def auto_init; end

      end

    end

    def tos1
      auto_init
      @g.paint1
    end

    def tos2
      auto_init
      @g.paint2
    end

  end


  window = JFrame.new "Turtle Graphics Alpha"
  window.get_content_pane.add panel, BorderLayout::CENTER
  window.set_default_close_operation JFrame::EXIT_ON_CLOSE
  window.set_preferred_size Dimension.new(440, 440)
  window.set_resizable true
  window.pack
  window.set_visible true

  #UIManager.setLookAndFeel UIManager.getSystemLookAndFeelClassName

  #panel.add BorderLayout::CENTER, JButton.new( "Click Me")
  #panel.add BorderLayout::SOUTH, JLabel.new( "I'm a Simple Program")


  #g.paint1

  panel.tos1


  STDIN::gets

  panel.tos2

  panel.g.draw_line 380, 80, 380, 200 

  #g.paint2

  STDIN::gets

  window.set_visible false
  window.dispose

end

