#!/usr/bin/env python3

"""
plot the power spectrum of a 
"""

import sys		 												# for command line argument
import array  												# for byte string unpacking
import numpy    											# for computation and fft
import matplotlib.pyplot as pyplot 		# for plotting


path = sys.argv[1]
len_waveform = 1000 # 1 us
rate = 200 * 1e6					  # 200 MHz
sampling = 5 * 1e-9					# 5 ns
cutoff = 10 * 1e6           # 50 MHz  -> 10 MHz -> 5 MHz

notch_start = 2.6 * 1e6
notch_stop  = 5.0 * 1e6

def get_frequency_range(rate, len_waveform):
	return numpy.linspace(0, rate/(2), (len_waveform)/2+1 )

def fft_filter(x, upper):
	fft = numpy.fft.rfft(x)
	fft[upper:] = 0   							# hard cutoff
	return numpy.fft.irfft( fft )

def fft_filter_with_notch(x, upper, rng):
	fft = numpy.fft.rfft(x)
	fft[upper:] = 0   							# hard cutoff
	fft[rng[0]:rng[1]] = 0   				# hard cutoff
	return numpy.fft.irfft( fft )
	
def waveform_stream(path, length):
  file = open( path, 'rb' )     #don't forget 'rb' for reading binary
  frame_read = 0
  try: 
  	while frame_read < 200:
  		byte_waveform = file.read(length)
  		if not byte_waveform: break
  		yield numpy.fromiter( array.array( 'b', byte_waveform ), dtype=numpy.int )
  		frame_read += 1
  finally:
    file.close()
    print("waveform_stream: read %i frames" % frame_read)

def save_waveform(x, wfm, path):
		pyplot.xlabel( 'time' )
		pyplot.ylabel( 'amplitude' )
		pyplot.gca().set_autoscale_on(False)
		pyplot.axis([x[0], x[-1], -128, 127])
		#pyplot.yaxis().set_major_formatter(matplotlib.ticker.FixedOrderFormatter(x[1]))
		pyplot.plot( x, wfm)
		pyplot.savefig(path)
		pyplot.clf()

def filter_waveform( x, upper, path, notch_range = []):
	num_wfm = 0
	for waveform in waveform_stream( path, len(x) ):
		#save_waveform( x, waveform, sys.argv[2] + '/waveform%i_raw.png' % num_wfm)
		#filtered = fft_filter(waveform, upper)
		filtered = fft_filter_with_notch(waveform, upper, notch_range)
		save_waveform( x, filtered, sys.argv[2] + '/waveform%i_filt_10_notch.png' % num_wfm)
		num_wfm += 1
		if num_wfm % 10 == 0: print("waveform %i" % num_wfm)
	return 0


time = numpy.linspace(0, sampling*len_waveform, len_waveform)

freq = get_frequency_range(rate, len_waveform)
cutoff_index = 0
while freq[cutoff_index+1] < cutoff: cutoff_index += 1

notch_i_s, notch_i_e = 0, 0
while freq[notch_i_s] < notch_start: notch_i_s += 1
while freq[notch_i_e] < notch_stop: notch_i_e += 1


filter_waveform( time, cutoff_index, path, [notch_i_s, notch_i_e])



