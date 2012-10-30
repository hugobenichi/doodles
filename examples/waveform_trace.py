#!/usr/bin/env python3


import sys		 												# for command line argument
import array  												# for byte string unpacking
import numpy    											# for computation and fft
import pylab													# fro color map
import matplotlib.pyplot as pyplot 		# for plotting


path = sys.argv[1]
len_waveform = 1000 				# 1 us
rate = 200 * 1e6					  # 200 MHz
sampling = 5 * 1e-9					# 5 ns

	
def waveform_stream(path, length, num_frame=200):
  file = open( path, 'rb' )     #don't forget 'rb' for reading binary
  frame_read = 0
  try: 
  	while frame_read < 100000:
  		byte_waveform = file.read(length)
  		if not byte_waveform: break
  		yield numpy.fromiter( array.array( 'b', byte_waveform ), dtype=numpy.int )
  		frame_read += 1
  finally:
    file.close()
    print("waveform_stream: read %i frames" % frame_read)


def accumulate_trace( time_axis, path):
	trace = numpy.zeros( (len(time_axis),256), dtype=numpy.dtype('d') )	
	mean = numpy.linspace(0,0, len(time))
	num_wfm = 0
	for waveform in waveform_stream( path, len(time_axis) ):
		mean += waveform
		for axis, height in enumerate(waveform):
			trace[axis][128-height] += 1.0  # the y axis is reversed in the pylab.imshow() routine
		num_wfm += 1
		if num_wfm % 100 == 0: print(num_wfm)
	for col in trace:
		max_col = max(col)
		for heigth, pixel in enumerate(col):
			col[heigth] /= max_col
	#trace_max = max( [ max(col) for col in trace ] )
	return [mean/num_wfm, trace]#/trace_max]


time = numpy.linspace(0, sampling*len_waveform, len_waveform)
mean_wfm, trace = accumulate_trace( time, path)
axis_range = [0, time[-1], -128, 127]


pyplot.xlabel( 'time' )
pyplot.ylabel( 'amplitude' )

pyplot.gca().set_autoscale_on(False)
pyplot.axis( axis_range )
pyplot.plot( time, mean_wfm)

pyplot.imshow(trace.transpose(), shape=(1000,256), extent=axis_range, interpolation='spline36', cmap=pylab.cm.Reds, aspect='auto')

pyplot.savefig(sys.argv[2] + '/mean_trace.png', dpi=300)
pyplot.show()


