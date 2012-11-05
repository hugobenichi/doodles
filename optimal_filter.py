#!/usr/bin/env python3 
########################################################
#                                                      #
#   script to generate the mean fft spectrum           #
#   and mean waveform from a binary file of            #
#   waveforms (signed char)                            #
#                                                      #
#	  creation:    2012/10/30                            #   
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
#output = sys.argv[3]
frame  = -1
length = 1000
to_plot= False
rate   = 5 * 1e-9


ampl = 106*2
orig = 320
rise = 1.0 / 0.13e-6
fall = 1.0 / 0.6e-6
dc   = -87


time = tes.waveform.time(rate, length)
freq = tes.waveform.freq(rate, length)


model = tes.model.pulse( len(time), ampl, orig, rise * rate, fall * rate ) + dc

pulse = tes.waveform.average.from_collection( 
          tes.waveform.read_binary( 
            path=input, length=length, frame=1000) )

#tes.plot.waveform( (time, model - dc, pulse - dc) )


noise_power = tes.waveform.spectrum()
for noise_wfm in tes.waveform.read_binary( path=noise, length=length, frame=1000):
	noise_power.add( noise_wfm-dc )

noise   = noise_power.compute()
signal  = tes.waveform.spectrum.from_collection([pulse-dc])

s_lin = numpy.power(signal/10,10)
n_lin = numpy.power(noise/10,10)

opt_lin = s_lin / (s_lin + n_lin)

optimal = 10*numpy.log10(opt_lin)

#tes.plot.spectrum( (freq, signal, noise)  )
tes.plot.waveform( (freq, opt_lin) )
tes.plot.spectrum( (freq,optimal), ylim=(-60,0)  )

