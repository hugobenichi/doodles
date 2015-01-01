

module LogoJR

  module Turtle

    #describe the behavior and logic of a turtle
    module Movement

      #minimal ammount needed to update x or y position
      Step_Min = 0.001

      public

      #check if the turtle is inside the screen and put it back inside it if not
      def inside_screen
        x_max, y_max = 440, 440
        x = 0     if self.x < 0 
        y = 0     if self.y < 0 
        x = x_max if self.x > x_max 
        y = y_max if self.y > y_max      
      end

      #the turtle moves in straight line in its current direction
      def forward len
        raise TypeError, "Numeric argument expected" unless len.is_a? Numeric
        x_up = len * Math.cos( self.angle * Math::PI/180.0 )
        y_up = len * Math.sin( self.angle * Math::PI/180.0 )
        self.x += x_up if x_up.abs > Step_Min
        self.y += y_up if y_up.abs > Step_Min 
        inside_screen
        save
      end

      #the turtle turns to the right
      def right ang
        raise TypeError, "Numeric argument expected" unless ang.is_a? Numeric
        self.angle -= ang
        self.angle -= 360 while self.angle > +360
        self.angle += 360 while self.angle < -360
        save
      end

      #the turtle goes to a given point
      def goto x, y
        raise TypeError, "Numeric argument expected" unless x.is_a? Numeric
        raise TypeError, "Numeric argument expected" unless y.is_a? Numeric
        self.x = x
        self.y = y
        inside_screen
        save
      end

      #the turtle goes back to its home
      def home
        Turtle::Home.keys.map{|name| self[name] = Turtle::Home[name]}
        save
      end

      #the turtle moves backward
      def backward len
        forward -len
      end

      #the turtle turns left
      def left ang
        right -ang
      end

      alias fd right
      alias bk right
      alias back right
      alias rt right
      alias lt right
      alias reset home

    end

  end

end


