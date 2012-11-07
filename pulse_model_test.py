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
frame  = 10000#-1 #2000
rate   = 5 * 1e-9
dc     = -87


time = tes.waveform.time(rate, length)
freq = tes.waveform.freq(rate, length)
avrg = tes.waveform.average.from_collection( 
         tes.waveform.read_binary( 
           path=input, length=length, frame=frame) 
       ) - dc
ref_wfm = avrg / max(avrg)


# prepare filter for band filtering
band_index = tes.waveform.freq_index(freq, 20 * 1e6)
filter_band = lambda wfm: tes.filter.lowpass(wfm, band_index)


# prepare filter for optimal filtering
optimal = numpy.loadtxt(sys.argv[2])
filter_optm = lambda wfm: tes.filter.arbitrary(wfm, optimal)


# prepare fitting template
def fitting(weight=None):
    #def fitter(wfm): return 128 + dc + tes.model.fit_amplitude( wfm, ref_wfm, weight )
    return lambda wfm: 128 + dc + tes.model.fit_amplitude( wfm, ref_wfm, weight )


# prepare weighting function
ampl, orig, rise, fall = 106*2, 320, 1.0 / 0.13e-6, 1.0 / 0.6e-6 
deriv = tes.model.derivate( len(time), ampl, orig, rise * rate, fall * rate )
m = -min(deriv)
for (i,x) in enumerate(deriv):
    if x > m: deriv[i] = m

weight1 = numpy.abs(deriv/m)
weight2 = numpy.abs(ref_wfm)
m = max(weight2)/2
for (i,x) in enumerate(weight2):
    if x > m: weight2[i] = m


# prepare histograms
hist_raw  = tes.histogram.builder(lambda wfm: 128 + dc + wfm[370])
hist_fit  = tes.histogram.builder(fitting())
hist_fitw = tes.histogram.builder(fitting(ref_wfm))
hist_fitd = tes.histogram.builder(fitting(weight2))


for waveform in tes.waveform.read_binary( path=input, length=length, frame=frame):
    dc_wfm = waveform-dc
    #dc_wfm = filter_band(dc_wfm)
    dc_wfm = filter_optm(dc_wfm)
    e0 = hist_raw.add(dc_wfm)  - dc
    e1 = hist_fit.add(dc_wfm)  - dc
    e2 = hist_fitw.add(dc_wfm) - dc
    e3 = hist_fitd.add(dc_wfm) - dc
    #tes.plot.waveform( (time, dc_wfm, ref_wfm*e1) )
    #tes.plot.waveform( (time, dc_wfm, ref_wfm*e1, ref_wfm*e2) )
    #tes.plot.waveform( (time, dc_wfm, ref_wfm*e1, ref_wfm*e2, ref_wfm*e3), ylim = (-10,200) )
    #print(e1, e2, e3)


volt = numpy.arange(0,256)
tes.plot.waveform( [volt,hist_raw.bins, hist_fit.bins], xlim=(50,250))#, save=output + "_fit_")
#tes.plot.waveform( [volt,hist_raw.bins, hist_fitw.bins], xlim=(50,250))#, save=output + "_fitw_")
#tes.plot.waveform( [volt,hist_raw.bins, hist_fitd.bins], xlim=(50,250))#, save=output + "_fitd_")


print( "raw:                   ", hist_raw.visibility(8) )
print( "fit:                   ", hist_fit.visibility(8) )
print( "weighted fit:          ", hist_fitw.visibility(8) )
print( "derivate weighted fit: ", hist_fitd.visibility(8) )


#numpy.savetxt( sys.argv[2] + "_raw.val", hist_raw )
#numpy.savetxt( sys.argv[2] + "_wfit.val", hist_fitw )


