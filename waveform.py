import numpy
import array

def time( sampling, samples ):
	"""
	returns an ordered numpy array of time points in an sampling interval
	specified by the sampling rate (in sec) and the number of sumples
	"""
	return numpy.linspace(0, sampling*samples, samples)

def freq( sampling, samples ):
	"""
	returns an ordered numpy array of the frequencies of an sampling interval
	specified by the sampling rate (in sec) and the number of sumples.
	The frequencies values are the diffrent spectral components of
	a real fft transformation in the nyquist band. 
	"""
	return numpy.linspace(0, 1.0/(2*sampling), (samples)/2+1 )

def time2freq( time ):
	"""
	return the frequency domain representation of a numpy array of time points,
	assuming real fft transformation
	"""
	sampling = time[-1] / len(time)  # do not use time[1] -> float division roundoff error
	return numpy.linspace( 0, 1.0/(2*sampling) , len(time)/2+1 )

def freq2time( freq ):
	"""
	return the time domain representation of a numpy array of frequency points,
	assuming real fft transformation
	"""
	sampling = 1.0/(2*freq[-1])
	length = (len(freq)-1)*2
	return numpy.linspace( 0, length*sampling, length )

def read_binary( path, length = 1000, frame = -1 ):
	"""
	reads a file of binary waveforms (signed char format)
	and converts them to numpy arrays, one waveform at a time.
	the 'length' parameter controls the number of points ver waveform.
	the 'frame' parameter controls the total number of waveforms to read.
	"""
	file = open( path, 'rb' )     #don't forget 'rb' for reading binary
	frame_read = 0
	try:
		while (frame < 0 or frame > frame_read):
			byte_waveform = file.read(length)
			if not byte_waveform: break
			waveform = numpy.fromiter( array.array( 'b', byte_waveform ), dtype=numpy.int )
			yield numpy.append( waveform, numpy.linspace(0,0,trailing) )
			frame_read += 1
	finally:
		file.close()

