#!/usr/bin/env python3
########################################################
#                                                      #
#   script which performs Gaussian waist fit           #
#   of (x,y) data obtained with a beam profiler        #
#   or knife-edge like device                          #
#                                                      #
#   creation:    October 2012                          #
#   homepage:    github.com/hugobenichi/gauss_waist    #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


help = """
  wais.py is a simple python 3 script for Gaussian beam waist analysis
  It takes as input argument a text file of the form:
    z_1 h_1 v_1
    z_2 h_2 v-2
    z_3 h_3 v_3
    ...
  where h_i and v_i are the full width 13.5% maximum values 
  of the Gaussian profil of your laser beam at position z_i, 
  horizontal and vertical. If the standard deviation of the 
  Gaussian profil is sigma, then full width 13.5% maximum is 
  equal to 4 sigma, or twice the waist at that position.

  the script will calculate the beam waist origin z_0 and waist
  size w_0 at origin by fitting the data to the standard formula
  w_0 * sqrt( 1 + (z-z_0)**2 lambda**2 / pi**2 w_0**2 )

  It ouputs on stdout the fitting results, plots the data with
  its fitted curves, and save the plot to a png file.
  Units: milimeters

  The wavelength is right now hard-coded in the script. Don't
  forget to customize the script to your wavelength ( and 
  include the M2 parameter if needed.)

  author      hugo benichi
  email       hugo.benichi@m4x.org
  copyright   2012 hugo benichi  
  version     0.95.2
  
  argument: 
    path/to/input/data/file     (1st place on the command line)

  options:
    -h,        --help            print this help
    -l=LEN,    --lambda=LEN      set the laser wavelength
    -m=X,      --m2=X            set the Msquared parameter
    -w=WIDTH,  --width=width     set the width of the measured waist.
    -o=WHERE,  --output=WHERE    set the path for saving output graph figure (default: same as input with ".png" tag)

  additional info @ github.com/hugobenichi/gauss_waist
"""

import os                                   # for file basename
import sys                                  # for exit() and argv
import math                                 # for Pi
import getopt                               # for command line parsing

import numpy                                # for computation
import scipy.optimize                       # idem
import matplotlib                           # for showing fit results
from   matplotlib import pyplot

try:
    input = sys.argv[1]
except IndexError:
    sys.stderr.write( "no path argument to input data file given\n" )
    sys.exit(22)


output      = sys.argv[1]
wavelth     = 396e-6            # 792e-6
m2          = 1.0
waist_size  = 2                 # check if using 13.5% or 50%


opt_short, opt_long = "o:l:w:a:h", [ "output=", "lambda=", "width=", "alpha=", "help"]
options, args = getopt.getopt( sys.argv[2:], opt_short, opt_long )


try:
    for option, value in options:
        if option in ('-h', '--help'):      print(help); sys.exit(0)
        if option in ('-o', '--output'):    output = value
        if option in ('-l', '--lambda'):    wavelth = float(value)
        if option in ('-w', '--width'):     waist_size = float(value)
        if option in ('-m', '--m2'):        m2 = float(value)
except TypeError:
    sys.stderr.write("bad argument")
    sys.exit(22)


coeff = m2 * wavelth / math.pi  # reduced wavelength parameter for Rayleigh range computation


x, h, v = zip(*[ [float(x) for x in line.split()] for line in open( input, 'r') ])

pts = 1024
x_min = 0
x_max = 1.1 * max(x)
y_max = 1.1 * max( h + v)

prop_lin  = lambda z, z0, alpha: 2*alpha * (z - z0) 
prop_nlin = lambda z, z0, w: 2*numpy.sqrt( w**2 + ( coeff * (z-z0)/ w )**2  )
report    = lambda arg_hash: print( "  %(typ)s   z_0 = %(pos).6f    w_0 = %(w).6f" % arg_hash)
                                              
strings = []

for pair in [ [h, 'horizontal', 'c'], [v, 'vertical', 'm'] ]:
    y, axis, color = pair
    print( "%s waist fitting" % axis )    
    try:
        [ pos, alpha ], var = scipy.optimize.curve_fit( prop_lin, x, y)
        w0 = coeff / alpha
        report( {'typ':'lin fit','pos':pos,'w':w0} )
        [ pos, w0 ], var = scipy.optimize.curve_fit( prop_nlin, x, y, [pos, w0])
        report( {'typ':'nlin fit','pos':pos,'w':w0} )
    
        mrayleigh = 1.5*( pos - w0**2/coeff )
        if x_min > mrayleigh:
            x_min = mrayleigh
        
        x_range = numpy.linspace( mrayleigh, x_max, pts)
        matplotlib.pyplotplot( x_range, prop_lin( x_range, pos, coeff / w0 ) , color + '--')
        matplotlib.pyplotplot( x_range, prop_nlin( x_range, pos, w0 ) , color + '-')
        matplotlib.pyplotplot( [pos,pos], [0,2*w0], 'k-' )
        strings.append( '%(ax)s:  z_0 = %(p).2f, w_0 = %(w).4f' % {'ax':axis, 'p':pos,'w':w0} )
    except RuntimeError as err:
        print( "  could not perform fit" )
        sys.stderr.write('ERROR: %s\n' % str(err))

m = 0.9
for s in strings:
    matplotlib.pyplottext( x_min + 0.05*(x_max-x_min), m * y_max, s )
    m -= 0.05


matplotlib.pyplotgca().set_autoscale_on(False)
matplotlib.pyplotaxis([x_min, x_max, 0, 1.1*max( h + v)])
matplotlib.pyplottitle( os.path.basename(sys.argv[1]) )
matplotlib.pyplotxlabel( 'waist position (mm)' )
matplotlib.pyplotylabel( 'waist size (mm)' )
matplotlib.pyplotplot( x, h, 'b+', x, v, 'r+' )
matplotlib.pyplotsavefig( sys.argv[1] + '.png')
matplotlib.pyplotshow()


