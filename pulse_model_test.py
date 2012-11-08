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


input_data, input_filter, input_pulse, output = sys.argv[1:5]
length = 1000
frame  = -1 #2000
rate   = 5 * 1e-9
dc     = -87


time = tes.waveform.time(rate, length)
freq = tes.waveform.freq(rate, length)


# prepare filter functions
filter_band    = tes.filter.from_lp_cutoff(tes.waveform.freq_index(freq, 50 * 1e6))
filter_optimal = tes.filter.from_transfer(numpy.loadtxt(input_filter))


# prepare weighting function
ref_wfm = numpy.loadtxt(input_pulse)
ref_wfm /= max(ref_wfm)
ampl, orig, rise, fall = 106*2, 320, 1.0 / 0.13e-6, 1.0 / 0.6e-6 
deriv = tes.model.derivate( len(time), ampl, orig, rise * rate, fall * rate )

aderiv = numpy.abs(deriv)

m = -min(deriv)
for (i,x) in enumerate(deriv):
    if x > m: deriv[i] = m

weight0 = ref_wfm
weight1 = numpy.abs(deriv/m)
weight2 = weight0 * weight1
weight3 = weight0 * aderiv

weight2 = numpy.abs(weight0)
weight2[450:] = 0

#tes.plot.waveform((time, weight0, weight2))

# prepare fitting template
def fitting(weight=None):
    return lambda wfm: tes.model.fit.amplitude(wfm, ref_wfm, weight)

def fittingdc(weight=None):
    return lambda wfm: tes.model.fit.floating_amplitude(wfm, ref_wfm, weight)

# prepare histograms
hist_raw      = tes.histogram.builder(lambda wfm: wfm[370])
hist_opt      = tes.histogram.builder(lambda wfm: wfm[370])
hist_opt_fit  = tes.histogram.builder(fitting(weight0))

hist_opt_fit2 = tes.histogram.builder() #lambda x: x)


offset = 128+dc

for wfm in tes.waveform.read_binary(path=input_data, length=length, frame=frame, dc=dc):
    opt = filter_optimal(wfm)
    e0 = hist_raw.add(wfm, offset)
    e1 = hist_opt.add(opt, offset)
    e2 = hist_opt_fit.add(opt, offset) - offset
    
    e3, lvl = tes.model.fit.floating_amplitude(wfm, ref_wfm, weight0)
    #e3, lvl = tes.model.fit.floating_amplitude(opt, ref_wfm, weight0)
    
    hist_opt_fit2.add(e3+lvl, offset)
    
    #e3, lvl = hist_opt_fit2.add(opt, offset)
    #if 150 > e2:
        #tes.plot.waveform( (time, wfm, opt, ref_wfm*e2), ylim = (-10,200) )
        #tes.plot.waveform( (time, wfm, weight2*e2, weight0*e3), ylim = (-10,200) )
    #print(e2, e3, lvl, e2 - e3)
    #tes.plot.waveform( (time, wfm, weight0*e2, weight0*(e3)+lvl), ylim = (-10,200) )

volt = numpy.arange(0,256)
#tes.plot.waveform( [volt,hist_raw.bins, hist_opt.bins, hist_opt_fit.bins], xlim=(50,250))
tes.plot.waveform( [volt,hist_raw.bins, hist_opt_fit.bins], xlim=(50,250))
#tes.plot.waveform( [volt,hist_opt.bins, hist_opt_fit.bins], xlim=(50,250))
tes.plot.waveform( [volt,hist_raw.bins, hist_opt_fit2.bins], xlim=(50,250))
tes.plot.waveform( [volt,hist_opt_fit.bins, hist_opt_fit2.bins], xlim=(50,250))

print( "raw:                   ", hist_raw.visibility(8) )
print( "opt:                   ", hist_opt.visibility(8) )
print( "opt fit:               ", hist_opt_fit.visibility(8) )
print( "opt fit floating dc:   ", hist_opt_fit2.visibility(8) )


#numpy.savetxt( sys.argv[2] + "_raw.val", hist_raw )
#numpy.savetxt( sys.argv[2] + "_wfit.val", hist_fitw )


