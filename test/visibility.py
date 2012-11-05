#!/usr/bin/env python3 
########################################################
#                                                      #
#   test script for tes.histogram.visibility           #
#                                                      #
#	  creation:    2012/11/05                            #   
#   based on:    github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import sys
import numpy
import tes.histogram
import tes.plot


volt   = numpy.arange(0,256)
histo  = numpy.loadtxt( sys.argv[1] )
shisto = tes.histogram.smooth(histo, 4)

#tes.plot.waveform([volt, histo, shisto])

pre = numpy.zeros(len(histo)+1)
aft = numpy.zeros(len(histo)+1)

pre[1:] += histo
pre[:-1] -= histo

aft[:-1] += histo
aft[1:]  -= histo

diff = (pre[1:] + aft[:-1] ) / 2

zeros = []

prev = diff[0]
for where, val in enumerate(diff):
	if val * prev < 0: zeros.append( where )
	prev = val
#print(zeros)

print( tes.histogram.visibility(histo, 12) )

tes.plot.waveform([volt, histo])



