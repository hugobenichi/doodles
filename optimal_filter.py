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
import tes
from   tes import waveform
from   tes import plot
from   tes import filter
from   tes import model


input  = sys.argv[1]
noise  = sys.argv[2]
output = sys.argv[3]
frame  = -1
length = 1000
to_plot= False
rate   = 5e-9
cutoff = 2e7

dc = -87

freq = tes.waveform.freq(rate, length)
time = tes.waveform.time(rate, length)

wfm_stream   = tes.waveform.read_binary( path=input, length=length, frame=frame, dc=dc)
noise_stream = tes.waveform.read_binary( path=noise, length=length, frame=frame, dc=dc)

pulse  = tes.waveform.average.from_collection(wfm_stream)
signal = tes.waveform.spectrum.from_collection([pulse-dc])
noise  = tes.waveform.spectrum.from_collection(noise_stream)
optim  = tes.filter.optimal.from_spectrum(signal, noise)

optim[tes.waveform.freq_index(freq, cutoff):]=0
optim /= max(optim)

tes.plot.spectrum((freq, optim), ylim=(0,1), ylabel="amplitude")
tes.plot.waveform((time, pulse))


numpy.savetxt( output + "_filter.val", optim )
numpy.savetxt( output + "_pulse.val", pulse )


