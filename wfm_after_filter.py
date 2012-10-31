#!/usr/bin/env python3 
########################################################
#                                                      #
#   script to generate the mean fft spectrum           #
#   and mean waveform from a binary file of            #
#   waveforms (signed char)                            #
#                                                      #
#	  creation:    2012/10/31                            #   
#   based on:    github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################

help = """
  this script attempts to read a binary file of waveforms
  encoded in signed char, filters it and then compute the
  average waveform and the stacked trace image of all
  filtered waveform
  
  argument: 
    path/to/input/binary/file    (1st place on the command line)

  options:
    -h,        --help            print this help
    -c=BAND,   --cutoff=BAND     set the passband upper frequency (in Hz)
    -l=LEN,    --length=LEN      set the number of data point in one waveform
    -f=FRAME,  --frame=FRAME     set the number of total waveforms to read
    -r=RATE,   --rate=RATE       set the sampling interval (in sec)
    -s=OUTPUT, --save=OUTPUT     set the path for saving output data (default: no ouput)
    -p,        --plot            show the different graphs

  comments:
    - you need to have the "tes" python package on your python path
    - if neither -s nor -p options are specified, nothing happens
    - data are saved in textual format using numpy.savetxt. check
      the end of the script for details

  additional info @ github.com/hugobenichi/tes
"""

import sys
import getopt
import numpy
import tes.waveform
import tes.plot
import tes.filter

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
cutoff = 10 * 1e6  # 10 MHz

opt_short = "c:r:f:l:s:ph"
opt_long  = [ "cutoff=", "rate=", "frame=", "length=", "save=", "plot", "help"]
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
		if option in ('-c', '--cutoff'):    cutoff  = float(value)
except TypeError:
	sys.stderr.write("bad argument")
	sys.exit(22)


mean_wfm = tes.waveform.average()
wfmtrace = tes.waveform.trace()

freq = tes.waveform.freq(rate, length)
time = tes.waveform.time(rate, length)


notch_start = 2.6 * 1e6
notch_stop  = 5.0 * 1e6
band_index, notch_i_s, notch_i_e = 0, 0, 0

while freq[band_index] < cutoff: band_index += 1
while freq[notch_i_s] < notch_start: notch_i_s += 1
while freq[notch_i_e] < notch_stop: notch_i_e += 1

for waveform in tes.waveform.read_binary( path=input, length = length, frame = frame):
	filtered = tes.filter.lowpass(waveform, band_index)
	mean_wfm.add(filtered)
	wfmtrace.add(filtered)

mean_wfm_data = [ time, mean_wfm.compute() ]
wfmtrace_data = [ time, wfmtrace.compute() ] # cannot be passed directly to numpy.savetxt

tes.plot.waveform( mean_wfm_data, output, show = to_plot )
tes.plot.trace( wfmtrace_data, output, show = to_plot )

if output is not None: 
	numpy.savetxt( output + "meanwfm" + ".val", mean_wfm_data )
	numpy.savetxt( output + "trace"    + ".val", wfmtrace_data[1] )


