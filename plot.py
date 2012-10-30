import numpy
import pylab
import matplotlib.pyplot as pyplot

def waveform(path, time, waveform, show = False):
	pyplot.clf()
	pyplot.xlabel( 'time' )
	pyplot.ylabel( 'amplitude' )
	pyplot.plot( time, waveform)
	pyplot.savefig( path)
	if show: pyplot.show()

def spectrum(path, freq, spectrum, show = False):
	"""
	assume spectrum data is given in decibels
	"""
	pyplot.clf()
	pyplot.gca().set_autoscale_on(False)
	pyplot.axis([freq[1], freq[-1], 0, 100])
	pyplot.xscale('log')
	pyplot.xlabel( 'frequency' )
	pyplot.ylabel( 'power (dB)' )
	pyplot.plot( freq, spectrum )
	pyplot.savefig( path)
	if show: pyplot.show()


def histogram(path, data, show = False, bin_num = 50):
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
	if show: pyplot.show()

def trace(path, time, waveform, show = False):
	pyplot.clf()
	axis_range = [0, time[-1], -128, 127]
	pyplot.xlabel( 'time' )
	pyplot.ylabel( 'amplitude' )
	pyplot.gca().set_autoscale_on(False)
	pyplot.axis( axis_range )
	pyplot.imshow(trace.transpose(), shape=(len(time),256), extent=axis_range, interpolation='spline36', cmap=pylab.cm.Reds, aspect='auto')
	pyplot.savefig(path, dpi=300)
	if show: pyplot.show()
