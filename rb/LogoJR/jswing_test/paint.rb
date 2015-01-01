include Java

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


module Drawing

  class Frame < javax.swing.JFrame

    import java.awt.Color   #white black red
    import java.awt.BasicStroke
    import java.awt.Dimension
    import java.awt.BorderLayout
    import java.awt.Graphics
    import java.awt.Graphics2D
    import java.awt.Rectangle
    import java.awt.geom.Line2D

    def initialize *args
      super *args

      @count = 0
      panel = javax.swing.JPanel.new
      panel.background = Color::black
      panel.set_layout BorderLayout.new

      get_content_pane.add panel, BorderLayout::CENTER
      set_default_close_operation javax.swing.JFrame::EXIT_ON_CLOSE
      set_preferred_size Dimension.new(440, 440)
      set_resizable false
      pack
      set_visible true  
    end

    def paint g
      puts @count += 1
      g.set_paint Color.red
      g.set_stroke BasicStroke.new 2.0
      g.draw Rectangle.new(10,10,100,100)
      g.draw_line 10, 10, 300, 300
      g.draw Line2D::Double.new( 300.0, 10.0, 300.0, 300.0 )
      g.drawString "Hello world !", 10, 50;
    end

  end

end


window = nil

javax.swing.SwingUtilities.later do
  window = Drawing::Frame.new "Turtle Graphics Alpha"
end


STDIN::gets
window.set_visible false
window.dispose


