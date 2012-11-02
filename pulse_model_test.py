#!/usr/bin/env python3 
########################################################
#                                                      #
#   script to generate the mean fft spectrum           #
#   and mean waveform from a binary file of            #
#   waveforms (signed char)                            #
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

length = 1000
rate   = 5 * 1e-9
cutoff = 10 * 1e6  # 10 MHz

ampl = 106*2
orig = 320
rise = 1.0 / 0.13e-6
fall = 1.0 / 0.6e-6
dc   = -86

time = tes.waveform.time(rate, length)
pulse = tes.model.pulse( len(time), 
                         ampl,
                         orig,
                         rise * rate,
                         fall * rate ) + dc

tes.plot.waveform( (time, pulse), None, show = True )
