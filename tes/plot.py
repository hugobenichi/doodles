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


def histogram(data, path = None, show = False, bins = 50):
	if path is not None or show:
		pyplot.clf()
		fig = pyplot.figure()
		ax = fig.add_subplot(111)
		n, bins, patches = ax.hist(data, bin_num, normed=0, facecolor='green', alpha=0.75)
		# hist uses numpy.histogram under the hood to create 'n' and 'bins'.
		# numpy.histogram returns the bin edges, so there will be 50 probability
		# density values in n, 51 bin edges in bins and 50 patches.  To get
		# everything lined up, we'll compute the bin centers
		bincenters = 0.5*(bins[1:]+bins[:-1])
		ax.set_xlabel('voltage')
		ax.set_ylabel('frequency')
		ax.set_xlim( min(bins), max(bins) )
		# add figure saving  + ".png"
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
