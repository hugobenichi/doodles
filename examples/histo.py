#!/usr/bin/env python3

"""
Make a histogram of normally distributed random numbers and plot the
analytic PDF over it
"""

import sys
import numpy
import matplotlib.pyplot as pyplot
import matplotlib.mlab as mlab

#mu, sigma = 100, 15
#x = mu + sigma * numpy.random.randn(10000)

data_path = sys.argv[1]

voltage =	[float(x) for x in open( data_path, 'r') ]

fig = pyplot.figure()
ax = fig.add_subplot(111)

# the histogram of the data
n, bins, patches = ax.hist(voltage, 50, normed=0, facecolor='green', alpha=0.75)

# hist uses numpy.histogram under the hood to create 'n' and 'bins'.
# numpy.histogram returns the bin edges, so there will be 50 probability
# density values in n, 51 bin edges in bins and 50 patches.  To get
# everything lined up, we'll compute the bin centers
bincenters = 0.5*(bins[1:]+bins[:-1])
# add a 'best fit' line for the normal PDF
#y = mlab.normpdf( bincenters, mu, sigma)
#l = ax.plot(bincenters, y, 'r--', linewidth=1)

ax.set_xlabel('voltage')
ax.set_ylabel('frequency')
#ax.set_title(r'$\mathrm{Histogram\ of\ IQ:}\ \mu=100,\ \sigma=15$')
ax.set_xlim( min(voltage), max(voltage) )
#ax.set_ylim(0, 1)
#ax.grid(True)

pyplot.show()
