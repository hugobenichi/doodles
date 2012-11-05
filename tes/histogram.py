import sys
import numpy
from .. import tes.filter # avoid name conflict with filter

def byte1():
	return numpy.linspace( 0, 0, 256 )

def add(histo, where):
	try:
		histo[128+where] += 1
	except IndexError:
		sys.srterr.write( "tes.histogram.add index err ", 128+where)

def smooth(histo, smoothing):
	return tes.filter.moving_average(histo, smoothing)[0:len(histo)]

def find_peaks(histo):
	"""
	return position of peaks in histo
	"""
	return []

def visibility(histogram, smoothing = 2):
	franges = tes.filter.smooth(histo, smoothing)
	return 1.0

