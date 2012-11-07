#!/usr/bin/env python3
########################################################
#                                                      #
#   simple test for functions of module tes.model      #
#                                                      #
#   creation:    2012/11/02                            #
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
from   tes import model
from   tes import filter
from   tes import histogram


input  = sys.argv[1]
output = sys.argv[3]
length = 1000
frame  = -1 #2000
rate   = 5 * 1e-9
dc     = -87


time = tes.waveform.time(rate, length)
freq = tes.waveform.freq(rate, length)
avrg = tes.waveform.average.from_collection( 
         tes.waveform.read_binary( path=input, length=length, frame=frame, dc=dc) )
ref_wfm = avrg / max(avrg)


# prepare filter functions
optimal_transfer = numpy.loadtxt(sys.argv[2])
optimal_transfer /= max(optimal_transfer)

filter_band = tes.filter.from_lp_cutoff(tes.waveform.freq_index(freq, 50 * 1e6))
filter_optimal = tes.filter.from_transfer(optimal_transfer)


# prepare weighting function
ampl, orig, rise, fall = 106*2, 320, 1.0 / 0.13e-6, 1.0 / 0.6e-6 
deriv = tes.model.derivate( len(time), ampl, orig, rise * rate, fall * rate )
m = -min(deriv)
for (i,x) in enumerate(deriv):
    if x > m: deriv[i] = m

weight0 = ref_wfm
weight1 = numpy.abs(deriv/m)
weight2 = 2*numpy.abs(ref_wfm)
m = max(weight2)/2
for (i,x) in enumerate(weight2):
    if x > m: weight2[i] = m

# prepare fitting template
def fitting(weight=None):
    return lambda wfm: tes.model.fit_amplitude(wfm, ref_wfm, weight)


# prepare histograms
hist_raw      = tes.histogram.builder(lambda wfm: wfm[370])
hist_opt      = tes.histogram.builder(lambda wfm: wfm[370])
hist_opt_fit  = tes.histogram.builder(fitting(weight0))


offset = 128+dc

for wfm in tes.waveform.read_binary(path=input, length=length, frame=frame, dc=dc):
    opt = filter_optimal(wfm)
    e0 = hist_raw.add(wfm, offset)
    e1 = hist_opt.add(opt, offset)
    e2 = hist_opt_fit.add(opt, offset) - offset
    #tes.plot.waveform( (time, wfm, opt, ref_wfm*e2), ylim = (-10,200) )


volt = numpy.arange(0,256)
tes.plot.waveform( [volt,hist_raw.bins, hist_opt.bins, hist_opt_fit.bins], xlim=(50,250))
tes.plot.waveform( [volt,hist_opt.bins, hist_opt_fit.bins], xlim=(50,250))

print( "raw:                   ", hist_raw.visibility(8) )
print( "opt:                   ", hist_opt.visibility(8) )
print( "opt fit:               ", hist_opt_fit.visibility(8) )


#numpy.savetxt( sys.argv[2] + "_raw.val", hist_raw )
#numpy.savetxt( sys.argv[2] + "_wfit.val", hist_fitw )


