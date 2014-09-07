module Analysis

  module MultiFrames
    
    attr_accessor :ax, :ap

    #apply mode to self and to ax and ap
    def multi_quads param  
      vic = self.get_quad(param)
      param[:center_v] = param[:center]

      puts "getting alice x quadrature"
      param[:center] = param[:center_ax]
      a_x  = @ax.get_quad param

      puts "getting alice p quadrature"
      param[:center] = param[:center_ap]
      a_p  = @ap.get_quad param 

      param[:center] = param[:center_v]
      [vic,a_x,a_p]
    end

    #return an array of colummns: phase, victor, alice_x, alice_p
    def multi_process param
      [process_phase(param)] + self.multi_quads(param)
    end

    def multi_to_marginal param
      Quix::Conditional::make_cond_margi *self.multi_process(param)
    end

  end

end
