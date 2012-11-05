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

help = """
  this script attempts to read a binary file of waveforms
  encoded in signed char and computes its mean fft spectrum
  it also computes the mean waveform and the trace image
  of all waveforms stacked together. 
  
  argument: 
    path/to/input/binary/file    (1st place on the command line)

  options:
    -h,        --help            print this help
    -l=LEN,    --length=LEN      set the number of data point in one waveform
    -f=FRAME,  --frame=FRAME     set the number of total waveforms to read
    -r=RATE,   --rate=RATE       set the sampling interval (in sec)
    -s=OUTPUT, --save=OUTPUT     set the path for saving output data (default: no ouput)
    -p,        --plot            show the fft spectrum

  comments:
    - you need to have the "tes" python package on your python path
    - if neither -s nor -p options are specified, nothing happens
    - spectrum and mean waveform data are saved in textual format 
      using numpy.savetxt.
        1st line: frequency axis, space seperated
        2nd line: frequency components in dB, space seperated
        use numpy.loadtxt("path/to/data") to reload the data

  additional info @ github.com/hugobenichi/tes
"""

import sys
import getopt
import numpy
import tes.waveform
import tes.plot

try:
	input = sys.argv[1]
except IndexError:
	sys.stderr.write( "no path argument to input binary file\n" )
	sys.exit(22)

output = None
frame  = -1
length = 1000
to_plot= False
rate   = 5 * 1e-9

opt_short, opt_long = "r:f:l:s:ph", [ "rate=", "frame=", "length=", "save=", "plot", "help"]
options, args = getopt.getopt( sys.argv[2:], opt_short, opt_long )

try:
	for option, value in options:
		if option in ('-h', '--help'):
			print(help)
			sys.exit(0)
		if option in ('-p', '--plot'):      to_plot = True
		if option in ('-l', '--length'):    length  = int(value)
		if option in ('-f', '--frame'):     frame   = int(value)
		if option in ('-s', '--save'):      output  = value
		if option in ('-r', '--rate'):      rate    = float(value)
except TypeError:
	sys.stderr.write("bad argument")
	sys.exit(22)


mean_wfm = tes.waveform.average()
spectrum = tes.waveform.spectrum()
wfmtrace = tes.waveform.trace()

for waveform in tes.waveform.read_binary( path=input, length=length, frame=frame):
	mean_wfm.add(waveform)
	spectrum.add(waveform) 
	wfmtrace.add(waveform)

mean_wfm_data = [ tes.waveform.time(rate, length), mean_wfm.compute() ]
spectrum_data = [ tes.waveform.freq(rate, length), spectrum.compute() ]
wfmtrace_data = [ tes.waveform.time(rate, length), wfmtrace.compute() ]

tes.plot.spectrum( spectrum_data, save=output, show=to_plot )
tes.plot.waveform( mean_wfm_data, save=output, show=to_plot )
tes.plot.trace( wfmtrace_data, save=output, show=to_plot )

if output is not None: 
	numpy.savetxt( output + "average"  + ".val", mean_wfm_data )
	numpy.savetxt( output + "spectrum" + ".val", spectrum_data )
	numpy.savetxt( output + "trace"    + ".val", wfmtrace_data[1] )


