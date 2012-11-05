#!/usr/bin/env python3 
########################################################
#                                                      #
#   simple test for functions of module tes.model      #
#                                                      #
#	  creation:    2012/11/02                            #   
#   based on:    github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import sys
import numpy
import tes.waveform
import tes.plot
import tes.model
import tes.filter
import tes.histogram


input  = sys.argv[1]
length = 1000
frame  = 1000
rate   = 5 * 1e-9
dc     = -87


time = tes.waveform.time(rate, length)
freq = tes.waveform.freq(rate, length)
avrg = tes.waveform.average.from_collection( 
         tes.waveform.read_binary( 
           path=input, length=length, frame=frame) 
       ) - dc

ref_wfm = avrg / max(avrg)


cutoff = 20 * 1e6
band_index = tes.waveform.freq_index(freq,cutoff)

hist_raw = tes.histogram.byte1()
hist_max = tes.histogram.byte1()
hist_fit = tes.histogram.byte1()
hist_fitw = tes.histogram.byte1()


for waveform in tes.waveform.read_binary( path=input, length=length, frame=-1):
	waveform = tes.filter.lowpass(waveform, band_index)
	dc_wfm = waveform-dc
	e = tes.model.fit_amplitude( dc_wfm, ref_wfm, dc )
	e2 = tes.model.fit_autoweight_amplitude( dc_wfm, ref_wfm, dc )
	#tes.plot.waveform( (time, dc_wfm, ref_wfm*e) )
	#tes.plot.waveform( (time, dc_wfm, ref_wfm*e, ref_wfm*e2) )
	tes.histogram.add( hist_raw, dc+dc_wfm[370] )
	tes.histogram.add( hist_fit, dc+e )
	tes.histogram.add( hist_fitw, dc+e2 )
	tes.histogram.add( hist_max, dc+max(dc_wfm) )

volt = numpy.arange(0,256)
#tes.plot.waveform( [volt, hist_raw, hist_fit, hist_fitw ], save=output )
tes.plot.waveform( [volt, hist_raw, hist_fit] )
tes.plot.waveform( [volt,hist_raw,hist_fitw] )

viz_1 = tes.histogram.visibility(hist_fit, 8)
viz_2 = tes.histogram.visibility(hist_fitw, 8)

print( viz_1, viz_2 )

#numpy.savetxt( sys.argv[2] + "_raw.val", hist_raw )
#numpy.savetxt( sys.argv[2] + "_wfit.val", hist_fitw )
