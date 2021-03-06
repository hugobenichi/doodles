########################################################
#                                                      #
#   plotting routines for waveforms/fft spectrum/...   #
#                                                      #
#   creation:    2012/10/29                            #
#   based on:    github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import numpy                        # for numpy array
import pylab                        # for color maps
import matplotlib.pyplot            # for plotting
from   matplotlib import pyplot as pyplot


# TODO(hugo) rewrite module by using decorator to factorize all the common stuff
#              for example, can factorize saving graph, saving data, ploting graph


def waveform(waveform_data, save=None, show=True, xlim=None, ylim=None):
    if save is not None or show:
        (x, to_plot) = (waveform_data[0], waveform_data[1:])
        pyplot.clf()
        pyplot.xlabel('time')
        pyplot.ylabel('amplitude')
        pyplot.xlim(x[1], x[-1])
        if xlim is not None: pyplot.xlim(*xlim)
        if ylim is not None: pyplot.ylim(*ylim)
        for y in to_plot: pyplot.plot(x, y)
        if save is not None: pyplot.savefig(save + "waveform.png")
        if show: pyplot.show()


def spectrum(spectrum_data, save=None, show=True, xlim=None, ylim=(0, 100)):
    """
    assume spectrum data is given in decibels
    """
    if save is not None or show:
        (x, to_plot) = (spectrum_data[0], spectrum_data[1:])
        pyplot.clf()
        pyplot.gca().set_autoscale_on(False)
        pyplot.xlim(x[1], x[-1])
        if xlim is not None: pyplot.xlim(*xlim)
        pyplot.ylim(ylim[0], ylim[1])
        pyplot.xscale('log')
        pyplot.xlabel('frequency')
        pyplot.ylabel('power (dB)')
        for y in to_plot: pyplot.plot(x, y)
        if save is not None: pyplot.savefig(save + "spectrum.png")
        if show: pyplot.show()


def trace(trace_data, save=None, show=True):
    if save is not None or show:
        time, trace = trace_data
        pyplot.clf()
        axis_range = [0, time[-1], -128, 127]
        pyplot.xlabel('time')
        pyplot.ylabel('amplitude')
        pyplot.gca().set_autoscale_on(False)
        pyplot.axis(axis_range)
        pyplot.imshow(trace.transpose(), shape=(len(time),256),
                      extent=axis_range, interpolation='spline36', 
                      cmap=pylab.cm.Reds, aspect='auto')
        if save is not None: pyplot.savefig(save + "trace.png", dpi=300)
        if show: pyplot.show()


def histogram(to_plot, save=None, show=True, bins=50):
    if save is not None or show:
        pyplot.clf()
        # options cumulative = True for cumulative distribution
        all_max = []
        for data in to_plot:
            #results = pyplot.hist(data, bins, facecolor='green', alpha=0.5)
            #results = pyplot.hist(data, bins, alpha=0.2, histtype='stepfilled')
            results = pyplot.hist(data, bins, alpha=0.8, histtype='step', normed=True)
            n, edges, patches = results
            all_max.append(max(n))
        bincenters = 0.5*(edges[1:]+edges[:-1])
        pyplot.xlabel('voltage')
        pyplot.ylabel('frequency')
        pyplot.xlim(min(edges), max(edges))
        pyplot.ylim(0, max(all_max))
        if save is not None: pyplot.savefig(save + "histo.png", dpi=300)
        if show: pyplot.show()


