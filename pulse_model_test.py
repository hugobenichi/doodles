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


import tes.waveform
import tes.plot
import tes.model

length = 1000
rate   = 5 * 1e-9

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


pulse2 = tes.model.pulse( len(time), 
                         ampl,
                         orig,
                         0.5*rise * rate,
                         fall * rate ) + dc
#tes.plot.waveform( (time, pulse, pulse2), None, show = True )


avrg = tes.waveform.average.from_collection( tes.waveform.read_binary( path=input, length=length, frame=frame) )

dc_lvl = numpy.linspace(dc,dc,len(avrg)


tes.plot.waveform( (time, avrg, dc_lvl), None, show=True )
