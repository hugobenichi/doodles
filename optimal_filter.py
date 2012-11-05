#!/usr/bin/env python3 
########################################################
#                                                      #
#   script to test tes.filter.optimal() and to         #
#   generate the optimal filter given a reference      #
#   signal and a noise signal                          #
#                                                      #
#   creation:    2012/10/30                            #
#   based on:    github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import sys
import numpy
import tes.waveform
import tes.plot
import tes.filter
import tes.model


input  = sys.argv[1]
noise  = sys.argv[2]
output = sys.argv[3]
frame  = -1
length = 1000
to_plot= False
rate   = 5e-9
cutoff = 2e7


freq = tes.waveform.freq(rate, length)
time = tes.waveform.time(rate, length)

if False:
	filt, sign, noiz = tes.filter.optimal(
		tes.waveform.read_binary( path=input, length=length, frame=frame),
		tes.waveform.read_binary( path=noise, length=length, frame=frame),
		dc
	)

	filt[tes.waveform.freq_index(freq, cutoff):] = 0

	tes.plot.spectrum( (freq, sign, noiz), save=output+"_spec")
	tes.plot.spectrum( (freq, filt), ylim=(0,1), save=output+"_filter")
	numpy.savetxt( output + "_filter.val", filt )


if True:
	ampl = 106*2
	orig = 320
	rise = 1.0 / 0.13e-6
	fall = 1.0 / 0.6e-6
	dc   = -87
	model = tes.model.pulse( len(time), ampl, orig, rise * rate, fall * rate ) + dc
	deriv = tes.model.derivate( len(time), ampl, orig, rise * rate, fall * rate )
	tes.plot.waveform((time,model,deriv, numpy.abs(model*deriv)) )

