########################################################
#                                                      #
#   module to estimate the timing characteristic       #
#   and mean waveform from a binary file of            #
#   waveforms (signed char)                            #
#                                                      #
#	  creation:    2012/11/01                            #   
#   project:     github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import sys                # for clean exit and stderr
import numpy              # for array calculation
import scipy.optimize     # for non-lin mean-square fit


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
	assuming a 0 background dc voltage
	"""
	x = numpy.arange(0-origin,length-origin)
	exp_rise = numpy.exp( -(rise* x ) )
	exp_fall = numpy.exp( -(fall* x ) )
	exp_rise[:origin] = 0
	exp_fall[:origin] = 0
	return amplitude * (exp_fall - exp_rise)


def derivate(length, amplitude, origin, rise, fall):
	"""
	compute the 1st order time derivate of a model
	pulse in a numpy array given estimated timing
	characteristic assuming a 0 background dc voltage
	"""
	x = numpy.arange(0-origin,length-origin)
	exp_rise = rise * numpy.exp( -(rise* x ) )
	exp_fall = fall * numpy.exp( -(fall* x ) )
	exp_rise[:origin] = 0
	exp_fall[:origin] = 0
	return amplitude * (exp_rise - exp_fall)

def fit_all_parameters( avg_waveform, dc ):
	"""
	fit an averaged waveform to the model by
	estimating the timing characteristic of 
	the pulse, given the background dc level.
	the model provides the shape to optimize.
	"""
	parameters = initial_guess( avg_waveform, dc )
	# 4-dim optimization is going to be really expensive ...
	# maybe do dc and amplitude optimization first
	# nested inside an origin, rising, falling optimization
	# or do MC guessing
	def try_pulse(x, a, o, r, f):
		return (pulse( len(avg_waveform), a, o, r, f )[x] + dc)
	try:
		parameters = scipy.optimize.curve_fit(
                   try_pulse, 
                   raw_waveform, 
                   range(0,len(avg_waveform)-1),
                   parameters )
	except RuntimeError as err:
		sys.stderr.write('could not perform fit ERROR: %s\n' % str(err))
		sys.exit(125)
	return parameters


def fit_amplitude( raw_waveform, ref_waveform, dc ):
	"""
	fit a single noisy waveform to the model given 
	previously estimated timing characteristic
		since the shape and timing characteristic are already set
		the x axis for the non-lin mean square fit is directly
		a model pulse, and the optimization function is simply
		a multiplier (it s a linear mean square fit)

		!! eventually can do this better with a per-frame
		   optimized dc level
	"""
	def difference(a): return raw_waveform - a * ref_waveform
	amplitude = max( raw_waveform )
	[amplitude], covar = scipy.optimize.leastsq( difference, [amplitude] )
	return amplitude


def fit_autoweight_amplitude( raw_waveform, ref_waveform, dc ):
	def difference(a): return ref_waveform * (raw_waveform - a * ref_waveform)
	amplitude = max( raw_waveform )
	[amplitude], covar = scipy.optimize.leastsq( difference, [amplitude] )
	return amplitude


def initial_guess( raw_waveform, dc ):
  """
	do a guess on all parameters according to the raw 
	averaged waveform in a "smart" way, provided the
  background dc voltage
  """
  amplitude = 1.0 # find with max
  origin = 0  # find with a threshold detection compared to max
  rise = 10.0 # find with rising time between origin and amplitude
  fall = 10.0 # find with falling time between max and relaxation amp 
  return (amplitude, time, rise, fall)


# !                                                        ! #
# ! try to fit a raw waveform against the average waveform ! #
# !                                                        ! #

