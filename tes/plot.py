import numpy
import pylab
import matplotlib.pyplot as pyplot

def waveform(waveform_data, path = None, show = False):
	if path is not None or show:
		pyplot.clf()
		pyplot.xlabel( 'time' )
		pyplot.ylabel( 'amplitude' )
		(x, to_plot) = (waveform_data[0], waveform_data[1:] )
		for y in to_plot: pyplot.plot(x, y)
		if path is not None: pyplot.savefig( path + "waveform.png")
		if show: pyplot.show()

def spectrum(spectrum_data, path = None, show = False):
	"""
	assume spectrum data is given in decibels
	"""
	if path is not None or show:
		freq, spectrum = spectrum_data
		pyplot.clf()
		pyplot.gca().set_autoscale_on(False)
		pyplot.axis([freq[1], freq[-1], 0, 100])
		pyplot.xscale('log')
		pyplot.xlabel( 'frequency' )
		pyplot.ylabel( 'power (dB)' )
		(x, to_plot) = (spectrum_data[0], spectrum_data[1:] )
		for y in to_plot: pyplot.plot(x, y)
		if path is not None: pyplot.savefig( path + "spectrum.png" )
		if show: pyplot.show()

def histogram(to_plot, path = None, show = False, bins = 50):
	if path is not None or show:
		pyplot.clf()
		# options cumulative = True for cumulative distribution
		for data in to_plot:
			#results = pyplot.hist(data, bins, facecolor='green', alpha=0.5)
			#results = pyplot.hist(data, bins, alpha=0.2, histtype='stepfilled')
			results = pyplot.hist(data, bins, alpha=0.8, histtype='step')
		n, edges, patches = results
		bincenters = 0.5*(edges[1:]+edges[:-1])
		pyplot.xlabel('voltage')
		pyplot.ylabel('frequency')
		pyplot.xlim( min(edges), max(edges) )
		if path is not None: pyplot.savefig(path + "histo.png", dpi=300)
		if show: pyplot.show()

def trace( trace_data, path = None, show = False):
	if path is not None or show:
		time, trace = trace_data
		pyplot.clf()
		axis_range = [0, time[-1], -128, 127]
		pyplot.xlabel( 'time' )
		pyplot.ylabel( 'amplitude' )
		pyplot.gca().set_autoscale_on(False)
		pyplot.axis( axis_range )
		pyplot.imshow( trace.transpose(), 
                   shape=(len(time),256), 
                   extent=axis_range, interpolation='spline36', 
                   cmap=pylab.cm.Reds, 
                   aspect='auto')
		if path is not None: pyplot.savefig(path + "trace.png", dpi=300)
		if show: pyplot.show()

