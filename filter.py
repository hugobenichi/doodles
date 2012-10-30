import numpy

def apply_filter( waveform, filter_action )
	fft = numpy.fft.rfft( waveform ) 
	filter_action(fft)
	return numpy.fft.irfft( fft )

def bandpass(waveform, array_of_range_pairs):
	def to_zero(fft):
		for start, end in array_of_range_pairs: fft[start:end] = 0
	return apply_filter(waveform, to_zero)

def lowpass(waveform, upper_index)
	def to_zero(fft): fft: fft[upper_index:] = 0
	return apply_filter(waveform, to_zero)

def lowpass(waveform, lower_index)
	def to_zero(fft): fft: fft[:lower_index] = 0
	return apply_filter(waveform, to_zero)

def arbitrary(waveform, transfer):
	def to_zero(fft): fft: fft = fft * transfer
	return apply_filter(waveform, to_zero)

def (waveform_collection, filter):
	"""
	think about iterator gluing idiom
	"""	
	return 0

# add generator of known filters with scipy ?
