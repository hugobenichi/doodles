#!/usr/bin/env python3 
########################################################
#                                                      #
#   simple test for histogram functions                #
#                                                      #
#	  creation:    2012/11/02                            #   
#   based on:    github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import tes.waveform
import tes.plot
import tes.model
import tes.filter
import sys
import numpy

input  = sys.argv[1]
length = 1000
frame  = -1 #10000
rate   = 5 * 1e-9

bins = 256
freq = tes.waveform.freq(rate, length)

histo_raw, histo_flt = [], []

histo = (   
  [[],380, 10 * 1e6],
  [[],380, 20 * 1e6]
)

def histinit(): return numpy.linspace( 0, 0, 256 )

histo_raw = histinit()
volt = numpy.arange(0,256)

for parameters in histo:
	index = 0
	while freq[index] < parameters[2]: index += 1
	parameters[2] = index

for waveform in tes.waveform.read_binary(path=input, length=length, frame=frame):
	histo_raw[ 128 + waveform[380] ] += 1
	#filtered = tes.filter.lowpass(waveform, band_index)
	#histo_flt.append( filtered[380] )
#	for (h, c, i) in histo:
		#h.append( filtered[c] )
#		h.append( tes.filter.lowpass(waveform, i)[c] )

#tes.plot.histogram( [histo_raw, histo_flt], show=True, bins=bins)
#tes.plot.histogram( [histo_raw] + [h for (h,c,i) in histo], show=True, bins=bins)
#tes.plot.histogram( [histo_raw] , show=True, bins=bins)

tes.plot.waveform( [volt, histo_raw ], show=True )

