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

time = tes.waveform.time(rate, length)
pulse = tes.model.pulse( len(time), 
                         1.0,
                         40,
                         0.20e-6,
                         0.66e-6 )


tes.plot.waveform( (time, pulse), None, show = True )
