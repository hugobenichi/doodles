import sys
import numpy
import scipy.optimize

"""
this module works in 2 steps
  1. estimate the pulse timing characteristic from the averaged waveform
  2. estimate the amplitude of individual waveform from the estimated
     timing characteristic
"""

def pulse(length, amplitude, origin, rise, fall):
	"""
	compute the waveform of a model pulse in a
  numpy array given estimated timing characteristic
	"""
	return numpy.linspace(amplitude, amplitude, length) 

def fit_all_parameters( avg_waveform ):
  """
  fit an average of waveform to estimate the timing
  characteristic of the pulse shape
  """
  parameters = initial_guess( avg_waveform )
	# 4-dim optimization is going to be really expensive ...
	def try_pulse(x, a, o, r, f):
		return pulse( len(avg_waveform), a, o, r, f )[x]
	try:
		parameters = scipy.optimize.curve_fit(
                   try_pulse, 
                   raw_waveform, 
                   range(0,len(avg_waveform)-1)
                   parameters )
	except RuntimeError as err:
		sys.stderr.write('could not perform fit ERROR: %s\n' % str(err))
		sys.exit(125)
	return parameters

def fit_amplitude( raw_waveform, parameters ):
  """
  fit a single noisy waveform to the model given 
	previously estimated timing characteristic
  """
	# to speed up I can replace x with basic pulse shape and 
	# use a simple multiplier function for the amplitude
	return 1.0 # 

def initial_guess( raw_waveform ):
  """
	do a guess on all parameters according to the raw 
	averaged waveform in a "smart" way
  """
  amplitude = 1.0 # find with max
  origin = 0  # find with a threshold detection compared to max
  rise = 10.0 # find with rising time between origin and amplitude
  fall = 10.0 # find with falling time between max and relaxation amp 
  return (amplitude, time, rise, fall)


	try:
		x = numpy.arange(0,10,0.1) # x values
		y = numpy.arange(0,10,0.1) # exp values
		pos, w0 = 1.0, 1.0
		prop_nlin = lambda z, z0, w: z+z0+w # first parameter is the x value
		[ pos, w0 ], var = scipy.optimize.curve_fit( prop_nlin, x, y, [pos, w0])

	except RuntimeError as err:
		sys.stderr.write('could not perform fit ERROR: %s\n' % str(err))
		sys.exit(125)

