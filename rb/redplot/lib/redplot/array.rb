# Author:       Hugo Benichi <hugo.benichi@m4x.org>
#
# Copyright:    Copyright (c) 2012 Hugo Benichi
#               You can redistribute and/or modify without restriction.
#
# Description:  creates a Monitor object that generate random data to be plotted
#               the data is plotted 'in real time' thanks to the callback interface to RedPlot
#      


require 'redplot'


class Array
  #this provide a quick hook for Array instances
  def plot!(args={})
    args[:as] ||= :plot
    args[:in]   = self.singleton_class
    data = self
    RedPlot::insert(args) { todraw.add args[:style]  {data} }
    wrapper = self.send args[:as]
  end
  #hook RedPlot to the array and draw what is inside it
  def plot_now!(args={})
    self.plot!(args).draw
  end
end
