

module LogoJR

  module Turtle

    #Struct that describe a turtle state
    State = Struct.new( *Attributes.keys ) do

      private

      #pack attributes values in an array
      def values
        self.members.map{ |name| self[name] }
      end

      #copy current attributes values inside a new TurtleState and save it in TurtleHistory
      def save
        raise NoMethodError, "no state history" unless self.respond_to? :history
        history.push  State.new(*values) #need to have a history method 
        self
      end

      #read a TurtleState attributes values and copy them in the current TurtleState
      def recall state
        raise TypeError, "Turtle::State argument expected" unless state.is_a? State
        self.members.each do |name| self[name] = state[name] end
        self
      end

      public

      #copy back TurtleHistory's last saved state in the turtle
      def undo
        raise NoMethodError, "no state history" unless self.respond_to? :history
        recall history.pop(2)[0] 
        save
      end
    
      #print attributes values to stdout
      def print
        STDOUT::puts  "turtle state = #{values}"
        self
      end

      alias report print

    end

  end

end
