########################################################
#                                                      #
#   linear filtering routines for waveforms            #
#                                                      #
#	  creation:    2012/10/29                            #   
#   based on:    github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import numpy                        # for numpy array


def apply_filter( waveform, filter_action ):
	fft = numpy.fft.rfft( waveform ) 
	filter_action(fft)
	return numpy.fft.irfft( fft )


def bandpass(waveform, array_of_range_pairs):
	def to_zero(fft):
		for start, end in array_of_range_pairs: fft[start:end] = 0
	return apply_filter(waveform, to_zero)


def lowpass(waveform, upper_index):
	def to_zero(fft): fft[upper_index:] = 0
	return apply_filter(waveform, to_zero)


def highpass(waveform, lower_index):
	def to_zero(fft): fft[:lower_index] = 0
	return apply_filter(waveform, to_zero)


def arbitrary(waveform, transfer):
	def to_zero(fft): fft = fft * transfer
	return apply_filter(waveform, to_zero)


def something(waveform_collection, filter):
	"""
	think about iterator gluing idiom
	waveform generator -> rfft generator -> filter -> irfft generator
	add forkign pipes
	"""	
	return 0


def moving_average(array, n):
	output = numpy.zeros(len(array) + n - 1)
	for i in range(0,n):
		output[i:len(array)+i] += array
	return (output/n) #[0:len(array)]


