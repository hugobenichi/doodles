require "./ruby/turtle/turtle"

include LogoJR

Screen::configure 440, 440

turtle = Turtle::create

puts turtle.class, turtle

wait = 0.3

turtle.forward( 10.0)
sleep wait
turtle.forward( 140.0)
sleep wait
turtle.right( 360+90)
sleep wait
turtle.forward( 20.0)
sleep wait
turtle.reset.left( 360+90)
sleep wait


turtle.history.print

puts "undoing operations"
turtle.undo.undo

turtle.history.print

STDIN::gets


