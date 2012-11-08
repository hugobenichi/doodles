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
import matplotlib
from   matplotlib import pyplot     # for plotting
#from   matplotlib import matplotlib.pyplot as matplotlib.pyplot


# TODO(hugo) rewrite module by using decorator to factorize all the common stuff
#            for example, can factorize saving graph, saving data, ploting graph


def label(x="", y=""):
    """decorators which adds labels to a plot (specified with the "(x|y)label" kw arg."""
    def label_decorator(routine):
        def add_label(*args, **kwargs):
            routine(*args, **kwargs)
            xlabel = kwargs["xlabel"] if "xlabel" in kwargs else x
            ylabel = kwargs["ylabel"] if "ylabel" in kwargs else y
            matplotlib.pyplot.xlabel(xlabel)
            matplotlib.pyplot.ylabel(ylabel)
        return add_label
    return label_decorator


def xlim(xlim=None):
    """decorator which adds control to the x axis range through the kw arg "ylim"."""
    def xlim_decorator(routine):
        def add_xlim(*args, **kwargs):
            routine(*args, **kwargs)
            if xlim is not None: matplotlib.pyplot.xlim(*xlim)
            if "xlim" in kwargs: matplotlib.pyplot.xlim(*kwargs["xlim"])
        return add_xlim
    return xlim_decorator


def ylim(ylim=None):
    """decorator which adds control to the y axis range through the kw arg "ylim"."""
    def ylim_decorator(routine):
        def add_ylim(*args, **kwargs):
            routine(*args, **kwargs)
            if ylim is not None: matplotlib.pyplot.ylim(*ylim)
            if "ylim" in kwargs: matplotlib.pyplot.ylim(*kwargs["ylim"])
        return add_ylim
    return ylim_decorator


def view(routine):
    """decorator which plots figure (with control to turn off display via the "show" kw arg)."""
    def add_view(*args, **kwargs):
        routine(*args, **kwargs)
        show = kwargs["show"] if "show" in kwargs else True  # default is to show the plot
        if show: matplotlib.pyplot.show()
    return add_view


def save(tag="", dpi=300):
    """decorator which adds control to save a graph with the kw arg "save"."""
    def save_decorator(routine):
        def add_save(*args, **kwargs):
            routine(*args, **kwargs)
            if "save" in kwargs and kwargs["save"] is not None:
                matplotlib.pyplot.savefig(kwargs["save"] + tag + ".png", dpi=dpi)
        return add_save
    return save_decorator


def log_scale(default=True):
    """decorator to control the scaling of the x-axis to log scale or not (kw arg "log")."""
    def log_scale_decorator(routine):
        def to_log_scale(*args,**kwargs):
            routine(*args,**kwargs)
            log = kwargs["log"] if "log" in kwargs else default
            if log: matplotlib.pyplot.xscale("log")
        return to_log_scale
    return log_scale_decorator


@save("waveform")
@view
@xlim()
@ylim()
@label("time", "amplitude")
def waveform(waveform_data, **kwargs):
    """plots a time domain waveform.
    waveform_data should be a list whose 1st element are the x-axis time points values."""
    _clear()
    _xlim_to(waveform_data)
    _plot(waveform_data)


@save("spectrum")
@view
@xlim()
@ylim((0,100))
@label("frequency", "power (dB)")
@log_scale(True)
def spectrum(spectrum_data, **kwargs):
    """plots a frequency domain waveform.
    spectrum_data_data should be a list whose 1st element are the x-axis frequency values.
    default yaxis settings assume spectrum data is given in decibels."""
    _clear()
    _xlim_to(spectrum_data)
    #matplotlib.pyplot.xscale('log')
    _plot(spectrum_data)


@save("trace")
@view
@label("time", "amplitude")
def trace(trace_data, **kwargs):
    """
    plots a trace of waveforms stacked together inside a matrix (ndarray).
    trace_data should be a 2-tuple whose 1st element are the x-axis frequency values
    and whose second element is the trace matrix to display."""
    _clear()
    matplotlib.pyplot.imshow(
        trace_data[1].transpose(), 
        #shape=(len(trace_data[0), 256),
        extent=[0, trace_data[0][-1], -128, 127], 
        interpolation='cubic', 
        cmap=pylab.cm.Reds, 
        aspect='auto'
    )


@save("histogram")
@view
@label("voltage", "frequency")
def histogram(to_plot, **kwargs):
    """plots an histogram. Should be made compatible with tes.histogram."""
    bins = kwargs["bins"] if "bins" in kwargs else 50
    _clear()
    all_max = []
    for data in to_plot:
        #results = matplotlib.pyplot.hist(data, bins, facecolor='green', alpha=0.5)
        #results = matplotlib.pyplot.hist(data, bins, alpha=0.2, histtype='stepfilled')
        results = matplotlib.pyplot.hist(data, bins, alpha=0.8, histtype='step', normed=True)
        n, edges, patches = results
        all_max.append(max(n))
    bincenters = 0.5*(edges[1:]+edges[:-1])
    matplotlib.pyplot.xlim(min(edges), max(edges))
    matplotlib.pyplot.ylim(0, max(all_max))


def _plot(plot_data): #shortcut
    for y in plot_data[1:]: matplotlib.pyplot.plot(plot_data[0],y)


def _clear(): # another shortcut
    matplotlib.pyplot.clf() 


def _xlim_to(plot_data): # yet another shortcut
    matplotlib.pyplot.xlim(plot_data[0][1], plot_data[0][-1])


