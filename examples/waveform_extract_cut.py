#!/usr/bin/env python3


import sys		 												# for command line argument
import array  												# for byte string unpacking
import numpy    											# for computation and fft


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

def write_cut( path, data):
	file = open( path + '/waveform_cut_x.dat', 'w')
	try:
		for val in data: file.writelines("%d\n" % val ) # quelle methode de merde
	finally:
		file.close()

def extract_cut( length, point, path):
	return [waveform[point] for waveform in waveform_stream( path, length )]

cut = extract_cut( 1000, int(sys.argv[3]), sys.argv[1])

#write_cut(sys.argv[2], cut)


import matplotlib.pyplot as pyplot
import matplotlib.mlab as mlab

outputpath = sys.argv[2] + '/hist_waveform_cut_x' + sys.argv[3] + '_bins' + sys.argv[4] + '.png'

voltage =	cut

fig = pyplot.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('voltage')
ax.set_ylabel('frequency')
ax.set_xlim( -128, 127 )
n, bins, patches = ax.hist(voltage, int(sys.argv[4]), normed=0, facecolor='green', alpha=0.75)
bincenters = 0.5*(bins[1:]+bins[:-1])
pyplot.savefig(outputpath)
pyplot.show()
