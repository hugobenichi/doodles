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
output = sys.argv[3]
length = 1000
frame  = 2000
rate   = 5 * 1e-9
dc     = -87


time = tes.waveform.time(rate, length)
freq = tes.waveform.freq(rate, length)
avrg = tes.waveform.average.from_collection( 
         tes.waveform.read_binary( 
           path=input, length=length, frame=frame) 
       ) - dc

ref_wfm = avrg / max(avrg)


ampl = 106*2
orig = 320
rise = 1.0 / 0.13e-6
fall = 1.0 / 0.6e-6
deriv = tes.model.derivate( len(time), ampl, orig, rise * rate, fall * rate )

m = -min(deriv)
for (i,x) in enumerate(deriv):
	if x > m: deriv[i] = m

weight = numpy.abs(deriv/m)

weight2 = numpy.abs(ref_wfm)
m = max(weight2)/2
for (i,x) in enumerate(weight2):
	if x > m: weight2[i] = m

cutoff = 20 * 1e6
band_index = tes.waveform.freq_index(freq,cutoff)

hist_raw = tes.histogram.byte1()
hist_fit = tes.histogram.byte1()
hist_fitw = tes.histogram.byte1()
hist_fitd = tes.histogram.byte1()

optimal = numpy.loadtxt(sys.argv[2])

for waveform in tes.waveform.read_binary( path=input, length=length, frame=frame):
	#waveform = tes.filter.lowpass(waveform, band_index)
	dc_wfm = waveform-dc
	#dc_wfm = tes.filter.arbitrary(waveform-dc, optimal)
	e1 = tes.model.fit_amplitude( dc_wfm, ref_wfm )
	e2 = tes.model.fit_weight_amplitude( dc_wfm, ref_wfm, ref_wfm )
	e3 = tes.model.fit_weight_amplitude( dc_wfm, ref_wfm, weight2 )
	#tes.plot.waveform( (time, dc_wfm, ref_wfm*e1) )
	#tes.plot.waveform( (time, dc_wfm, ref_wfm*e1, ref_wfm*e2) )
	#tes.plot.waveform( (time, dc_wfm, ref_wfm*e1, ref_wfm*e2, ref_wfm*e3), ylim = (-10,200) )
	#print(e1, e2, e3)
	tes.histogram.add( hist_raw, dc+dc_wfm[370] )
	tes.histogram.add( hist_fit, dc+e1 )
	tes.histogram.add( hist_fitw, dc+e2 )
	tes.histogram.add( hist_fitd, dc+e3 )

volt = numpy.arange(0,256)
tes.plot.waveform( [volt, hist_raw, hist_fit, hist_fitw ])#, save=output )
tes.plot.waveform( [volt,hist_raw, hist_fit], xlim=(50,250))#, save=output + "_fit_")
tes.plot.waveform( [volt,hist_raw,hist_fitw], xlim=(50,250))#, save=output + "_fitw_")
tes.plot.waveform( [volt,hist_raw,hist_fitd], xlim=(50,250))#, save=output + "_fitd_")

print( "raw:                   ", tes.histogram.visibility(hist_raw, 8) )
print( "fit:                   ", tes.histogram.visibility(hist_fit, 8) )
print( "weighted fit:          ", tes.histogram.visibility(hist_fitw, 8) )
print( "derivate weighted fit: ", tes.histogram.visibility(hist_fitd, 8) )


#numpy.savetxt( sys.argv[2] + "_raw.val", hist_raw )
#numpy.savetxt( sys.argv[2] + "_wfit.val", hist_fitw )
