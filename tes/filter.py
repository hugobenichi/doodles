########################################################
#                                                      #
#   linear filtering routines for waveforms            #
#                                                      #
#   creation:    2012/10/29                            #
#   based on:    github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import numpy                        # for numpy array
import tes
from   tes import waveform          # for average and spectrum in optimal


# TODO(hugo) think about iterator/stream gluing idiom: wfm stream -> rfft stream -> map filter -> irfft stream


def in_frequency(transformation):
    """Decorator which apply numpy.fft.rfft to the first argument of transformation.
    The result of transformation is transformed back in the time domain with numpy.fft.irfft.
    """
    def transform_in_frequency( *args, **kwargs ):
        return numpy.fft.irfft(transformation(numpy.fft.rfft(args[0]), *args[1:], **kwargs))
    return transform_in_frequency


def from_transfer(transfer):
    """generate a filter function from a transfer function specified as an ndarray.
    the returned function takes as input a waveform at a time (ndarray)."""
    @in_frequency
    def mult(wfm): return wfm * transfer
    return mult


def from_lp_cutoff(upper_index):
    return lambda wfm: lowpass(wfm, upper_index)


def from_hp_cutoff(lower_index):
    return lambda wfm: highpass(wfm, lower_index)


@in_frequency
def lowpass(waveform, upper_index):
    waveform[upper_index:] = 0
    return waveform


@in_frequency
def highpass(waveform, lower_index):
    waveform[:lower_index] = 0
    return waveform


class optimal:          # just a namespace
    """Computes and returns (ndarray) the Wiener optimal linear filter from a collection of 
    waveforms to average which represents the signal, and a collection of waveform of the 
    background noise.
    """

    # TODO(hugo) correct in the user script the DC removal which was in the prev version
    def from_collection(waveform_col, noise_col):
        pulse  = tes.waveform.average.from_collection(waveform_col)
        signal = tes.waveform.spectrum.from_collection([pulse])
        noise  = tes.waveform.spectrum.from_collection(noise_col)
        return optimal.from_spectrum(signal, noise)
    
    def from_spectrum(signal,noise):
        slin, nlin = numpy.power(signal/10, 10), numpy.power(noise/10, 10)
        optimal = slin / (slin+nlin)
        optimal /= max(optimal)
        return (optimal, signal, noise)


def moving_average(array, n):
    """Moving average filter which returns a ndarray with len len(input) + n - 1."""
    def moving_average_filter(array):
        output = numpy.zeros(len(array) + n - 1)
        for i in range(n):
            output[i:len(array)+i] += array
        return (output/n)
    return moving_average_filter


