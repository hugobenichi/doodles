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


input  = sys.argv[1]
length = 1000
frame  = 1000

rate   = 5 * 1e-9

ampl = 106*2
orig = 320
rise = 1.0 / 0.13e-6
fall = 1.0 / 0.6e-6
dc   = -87

time = tes.waveform.time(rate, length)
freq = tes.waveform.freq(rate, length)

if False:  # produce and plot a model pulse from "hand-estimated" parameters
	pulse = tes.model.pulse( len(time), ampl, orig, rise * rate, fall * rate ) + dc
	tes.plot.waveform( (time, pulse), show = True )

# compare the hand-estimated dc-voltage to the average waveform
avrg = tes.waveform.average.from_collection( tes.waveform.read_binary( path=input, length=length, frame=frame) )
dc_lvl = numpy.linspace(dc,dc,len(avrg))
#tes.plot.waveform( (time, avrg-dc_lvl), show=True )


ref_waveform  = avrg - dc
ref_waveform /= max(ref_waveform)
#tes.plot.waveform( (time, ref_waveform), show=True )


cut = 370
def histinit(): return numpy.linspace( 0, 0, 256 )
hist_raw = histinit()
hist_max = histinit()
hist_fit = histinit()
hist_fit_weight = histinit()
volt = numpy.arange(0,256)


cutoff = 10 * 1e6
band_index = 0
while freq[band_index] < cutoff: band_index += 1


for waveform in tes.waveform.read_binary( path=input, length=length): #, frame=10000):
	waveform = tes.filter.lowpass(waveform, band_index)
	dc_wfm = waveform-dc
	e = tes.model.fit_amplitude( dc_wfm, ref_waveform, dc )
	e2 = tes.model.fit_autoweight_amplitude( dc_wfm, ref_waveform, dc )
	#tes.plot.waveform( (time, dc_wfm, ref_waveform*e), show = True)
	#tes.plot.waveform( (time, dc_wfm, ref_waveform*e, ref_waveform*e2), show = True)
	if True:
		try:
			hist_raw[128+dc+dc_wfm[cut]] += 1
			hist_fit[128+dc+e] += 1
			hist_fit_weight[128+dc+e2] += 1
			hist_max[128+dc+max(dc_wfm)] += 1
		except IndexError:
			print( "index err ", 128+e, 128+e2)
	#		tes.plot.waveform( (time, dc_wfm, ref_waveform*e), show = True)
#bins = 500 #1000
#tes.plot.histogram( [hist_raw, hist_fit], show=True, bins=bins)

#tes.plot.waveform( [volt, hist_raw, hist_fit, hist_fit_weight ], show=True )
tes.plot.waveform( [volt, hist_raw, hist_fit_weight ], path=sys.argv[2]+ "png", show=True )

