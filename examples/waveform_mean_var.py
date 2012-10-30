#!/usr/bin/env python3

"""
plot the power spectrum of a 
"""

import sys		 												# for command line argument
import array  												# for byte string unpacking
import numpy    											# for computation and fft
import matplotlib.pyplot as pyplot 		# for plotting


path = sys.argv[1]
len_waveform = 1000
rate = 1e-9

	
def waveform_stream(path, length):
  file = open( path, 'rb' )     #don't forget 'rb' for reading binary
  frame_read = 0
  try: 
  	while True:
  		byte_waveform = file.read(length)
  		if not byte_waveform: break
  		yield numpy.fromiter( array.array( 'b', byte_waveform ), dtype=numpy.int )
  		frame_read += 1
  finally:
    file.close()
    print("waveform_stream: read %i frames" % frame_read)

def read_waveform(path, waveform_length=1000):
	mean = numpy.linspace(0,0,waveform_length)
	var  = numpy.linspace(0,0,waveform_length)
	total_frame = 0
	for waveform in waveform_stream( path, waveform_length ):
		total_frame += 1
		mean += waveform
		var  += waveform*waveform	
	mean /= total_frame
	var  /= total_frame
	return [mean, ( var-(mean*mean) )/total_frame ]


time = numpy.linspace(0, rate*len_waveform, len_waveform)
mean, var = read_waveform(path, len_waveform)



#pyplot.gca().set_autoscale_on(False)
#pyplot.axis([0, freq[-1], 0.001, 100]) #1.1*max( mean_spectrum )/2])
#pyplot.yscale('log')
pyplot.xlabel( 'time' )
pyplot.ylabel( 'amplitude' )
pyplot.plot( time, mean)#, time, var )
pyplot.savefig( sys.argv[1] + '.mean_wfm.png')
pyplot.show()
pyplot.plot( time, var )
pyplot.savefig( sys.argv[1] + '.var_wfm.png')
pyplot.show()


