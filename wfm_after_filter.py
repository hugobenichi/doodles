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
    - you need to have the "tes" python package on your python path ()
    - if neither -s nor -p options are specified, nothing happens
    - spectrum data are saved in textual format using numpy.savetxt.
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
wfmtrace = tes.waveform.trace()

freq = tes.waveform.freq(rate, length)
time = tes.waveform.time(rate, length)


cutoff = 10 * 1e6           # 50 MHz  -> 10 MHz -> 5 MHz
notch_start = 2.6 * 1e6
notch_stop  = 5.0 * 1e6

band_index, notch_i_s, notch_i_e = 0, 0, 0

while freq[band_index+1] < cutoff: band_index += 1
while freq[notch_i_s] < notch_start: notch_i_s += 1
while freq[notch_i_e] < notch_stop: notch_i_e += 1

for waveform in tes.waveform.read_binary( path=input, length = length, frame = frame):
	filtered = tes.filter.lowpass(waveform)
	mean_wfm.add(filtered)
	wfmtrace.add(filtered)

mean_wfm_data = [ time, mean_wfm.compute() ]
wfmtrace_data = [ time, wfmtrace.compute() ]

tes.plot.waveform( mean_wfm_data, output, show = to_plot )
tes.plot.trace( wfmtrace_data, output, show = to_plot )

if output is not None: 
	numpy.savetxt( output + "meanwfm" + ".val", mean_wfm_data )
	numpy.savetxt( output + "trace"    + ".val", wfmtrace_data )


