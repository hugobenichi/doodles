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

# use decorator for pre_fft stream and post irfft stream

def _apply_filter(waveform, filter_action):
    """

    """
    fft = numpy.fft.rfft(waveform) 
    filter_action(fft)
    return numpy.fft.irfft(fft)


def bandpass(waveform, array_of_range_pairs):
    def to_zero(fft):
        for start, end in array_of_range_pairs: fft[start:end] = 0
    return _apply_filter(waveform, to_zero)


def lowpass(waveform, upper_index):
    def to_zero(fft): fft[upper_index:] = 0
    return _apply_filter(waveform, to_zero)


def highpass(waveform, lower_index):
    def to_zero(fft): fft[:lower_index] = 0
    return _apply_filter(waveform, to_zero)


def arbitrary(waveform, transfer):
    def to_zero(fft): fft = fft * transfer
    return _apply_filter(waveform, to_zero)


def optimal(waveform_col, noise_col, dc):
    pulse  = tes.waveform.average.from_collection(waveform_col)
    signal = tes.waveform.spectrum.from_collection([pulse - dc])
    noise  = tes.waveform.spectrum.from_collection([wfm - dc for wfm in noise_col])
    slin, nlin = numpy.power(signal/10, 10), numpy.power(noise/10, 10)
    optimal = slin / (slin+nlin)
    return (optimal, signal, noise)


def moving_average(array, n):
    output = numpy.zeros(len(array) + n - 1)
    for i in range(n):
        output[i:len(array)+i] += array
    return (output/n) #[0:len(array)]


