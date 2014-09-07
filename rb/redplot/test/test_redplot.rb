# Author:       Hugo Benichi <hugo.benichi@m4x.org>
#
# Copyright:    Copyright (c) 2012 Hugo Benichi
#               You can redistribute and/or modify without restriction.
#
# Description:  script that tests RedPlot functionalities
#
# How it works: just run 'ruby test/test_redplot' from the RedPlot directory
#               you need to have gnuplot in your path
#

require 'test/unit'
require 'shoulda'
require 'redplot'


PLOT_FOR_REAL   = false
WAIT_AFTER_PLOT = false

#test basic fonctionalities of the ToDraw helper class
class ToDrawTest < Test::Unit::TestCase
  
  context 'An instance of ToDraw' do

    def setup
      @todraw = RedPlot::ToDraw.new
    end

    should 'not burn down when used' do
      @todraw.reinit
      @todraw.add('w l title "tos"') do puts "tos" end
    end

    should 'have a pool of options and a pool of callable blocks' do
      @todraw.add('0') do 0 end
      @todraw.add('1') do 1 end
      @todraw.add('2') do 2 end
      assert_equal ['0','1','2'], @todraw.options
      assert_equal [0,1,2], @todraw.callbacks.map{ |b| b.call }
    end

    should 'return itself after "add" calls' do
      assert_equal @todraw.add{0}, @todraw
    end

    should 'be foolproof against empty block "add" calls' do
      @todraw.reinit
      @todraw.add('0')
      assert_equal @todraw.callbacks, []
    end

    should 'be foolproof against empty option "add" calls' do
      @todraw.reinit
      @todraw.add do 1 end
      assert_equal @todraw.callbacks.length, 1
      assert_equal @todraw.options, [""]
    end

  end

end


#test basic functionalities of Plotter wrapper object
class PlotterTest < Test::Unit::TestCase

  context 'A typical Plotter instance' do

    def setup
      @inf = RedPlot::Plotter.new
      RedPlot::Plotter.class_eval { attr_reader :proc }
    end

    should 'be initialized to default' do
      assert_equal  @inf.header, [],                                "empty header" 
      assert        @inf.todraw.is_a?(RedPlot::ToDraw),             "empty todraw" 
      assert_equal  @inf.path, 'redplot',                           "default path" 
      assert_equal  @inf.command, 'plot',                           "default command" 
      assert_nil    @inf.proc,                                      "no process"
    end

    should 'have lazy initialization of its gnuplot process' do                 
      assert_nil    @inf.proc
      @inf.start
      assert_not_nil @inf.proc
    end

    should 'accept new header arguments' do
      @inf.header << 'set xrange [0:5]'
      @inf.header = []
      @inf.header.delete_if { |line| lime.include? "log"}
    end

    should 'have a modifiable path' do
      @inf.path = "data"
    end

    should 'have a modifiable plot command' do
      @inf.command = "splot"
    end

    should 'have a puts and print command' do
      @inf.command = "splot"
    end

    should 'be garbaged collected' do

    end      

  end

end


#tests the inclusion mecanism of the RedPlot module
class RedPlotInclusionTest < Test::Unit::TestCase

  context 'The RedPlot module' do

    def assert_variables object
      assert object.respond_to?(:gnuplot_wrapper),  "has gnup_Plotter attr_reader"
      assert object.respond_to?(:plot),             "has plot method"
    end
  
    should 'be includable in classes' do
      assert_variables Class.new{include RedPlot}.new
    end

    should 'be extandable by objects' do
      assert_variables Object.new.extend(RedPlot)
    end

    should 'be inserable anywhere with any name' do
      target_class = Class.new
      RedPlot::insert in: target_class, as: :another_name 
      target = target_class.new
      assert      target.respond_to?(:gnuplot_wrapper),   "has gnup_Plotter attr_reader"
      assert      target.respond_to?(:another_name),      "has :another_name method"
      assert      target.respond_to?(:plot).!,            "does not have plot method"
    end

  end

  context 'a enhanced object' do

    def setup
      @target = Object.new.extend(RedPlot)     
    end

    should 'have lazy instanciation of its Plotter object' do
      assert_nil      @target.gnuplot_wrapper,    "nil via attr_reader"
      assert_not_nil  @target.plot,               "non nil via shorcut reader"
      assert_not_nil  @target.gnuplot_wrapper,    "non nil via attr_reader"
      assert @target.plot.is_a? RedPlot::Plotter
    end

  end

end


#test plotting functionalities of the RedPlot module
class RedPlotTest < Test::Unit::TestCase

  context 'for debug' do

    def setup
      @inf = RedPlot::Plotter.new
      @inf.instance_eval do 
        #@proc = STDOUT
      end
      @inf.todraw.add("w l") { [0,1,2,3] }
      @inf.todraw.add("w l") { [4.1,4.2,4.3,4.5,4.6] }
      @inf.todraw.add("w l") { [ [0,1,2,3], [rand,rand,rand,rand] ] }
      @inf.todraw.add("w l") { [ [0,1], [1,2], [2,0.5], [3,1.3] ] }
    end

    should 'do something when calling draw' do
      if PLOT_FOR_REAL 
        @inf.draw
        STDIN.gets if WAIT_AFTER_PLOT
      end
    end

  end

  #
  # I should test in more details Plotter#format_command and Plotter#format_data
  #

end


class ArrayFunctionalitiesTest < Test::Unit::TestCase

  context 'An array' do
  
    require 'redplot/array'

    should 'be enhancable by RedPlot' do
      ary = [0,1,2,3]
      assert        ary.plot!.is_a? RedPlot::Plotter
      assert        ary.gnuplot_wrapper.is_a? RedPlot::Plotter
      assert_equal  ary.gnuplot_wrapper, ary.plot
    end

  end

  context 'An enhanced array' do

    def setup
      @ary = [0,1,2,3]
      @ary.plot!
    end

    should 'draw itself' do
      if PLOT_FOR_REAL
        @ary.plot.draw
        STDIN.gets if WAIT_AFTER_PLOT
      end
    end

  end

  context 'An enhanced array' do

    should 'draw itself' do
      if PLOT_FOR_REAL
        [0,1,2,3].plot!.draw
        STDIN.gets if WAIT_AFTER_PLOT
      end
    end

  end

  context 'An enhanced array of arrays' do

    def setup
      len = 10
      @ary = [ Array.new(len){|i|i}, Array.new(len){rand} ]
      @ary.plot!(options: "w l title 'demo'")
    end

    should 'draw its arrays' do
      if PLOT_FOR_REAL
        @ary.gp.draw
        STDIN.gets if WAIT_AFTER_PLOT
      end
    end

    should 'save its data and script to disc' do
      @ary.plot.path = '%s/test/some_ary' % Dir.getwd
      @ary.plot.save_data.save_script
    end

    should 'save more data if todraw.add is used' do
      @ary.plot.todraw.add("w l title 'draw again'") do @ary end
      @ary.plot.path = '%s/test/some_other_ary' % Dir.getwd
      @ary.plot.save_data
    end

    should 'save its graph in png and ps formats' do
      @ary.plot.path = '%s/test/some_graph' % Dir.getwd
      @ary.plot.save_png.save_eps
    end

  end

end




P = RedPlot::Plotter.new
P.set.range '[0:1]'
P.unset
P.log 'x'
P.log.y

P.header.each do |line| puts line end


