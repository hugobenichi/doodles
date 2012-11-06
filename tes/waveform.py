########################################################
#                                                      #
#   script to generate the mean fft spectrum           #
#   and mean waveform from a binary file of            #
#   waveforms (signed char)                            #
#                                                      #
#   creation:    2012/10/27                            #
#   project:     github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import numpy                        # for numpy array
import array                        # for signed char unpacking


# TODO(hugo) move this to top namespace
def time(sampling, samples):
    """
    returns an ordered numpy array of time points in an sampling interval
    specified by the sampling rate (in sec) and the number of sumples
    """
    return numpy.linspace(0, sampling*samples, samples)


# TODO(hugo) move this to top namespace
def freq(sampling, samples):
    """
    returns an ordered numpy array of the frequencies of an sampling interval
    specified by the sampling rate (in sec) and the number of sumples.
    The frequencies values are the diffrent spectral components of
    a real fft transformation in the nyquist band. 
    """
    return numpy.linspace(0, 1.0/(2*sampling), (samples)/2+1)


# TODO(hugo) move this to top namespace
def time2freq(time):
    """
    return the frequency domain representation of a numpy array of time points,
    assuming real fft transformation
    """
    sampling = time[-1] / len(time)  # do not use time[1] -> float division roundoff error
    return numpy.linspace(0, 1.0/(2*sampling) , len(time)/2+1)


# TODO(hugo) move this to top namespace
def freq2time(freq):
    """
    return the time domain representation of a numpy array of frequency points,
    assuming real fft transformation
    """
    sampling = 1.0/(2*freq[-1])
    length = (len(freq)-1)*2
    return numpy.linspace(0, length*sampling, length)


# TODO(hugo) move this to top namespace
def freq_index(freq, where):
    """return the index in array <freq> of the frequency value <where>"""
    index = 0
    while freq[index] < where: index += 1
    return index


# TODO(hugo) use decorator to pass this into asyn fetcher
def read_binary(path, length=1000, frame=-1):
    """
    reads a file of binary waveforms (signed char format)
    and converts them to numpy arrays, one waveform at a time.
    the 'length' parameter controls the number of points ver waveform.
    the 'frame' parameter controls the total number of waveforms to read.
    """
    try:
        file = open(path, 'rb')     #don't forget 'rb' for reading binary
    except IOError:
        sys.stderr.write("error while opening binary file at %s\n" % path)
        sys.exit(2)
    frame_read = 0
    try:
        while (frame < 0 or frame > frame_read):
            byte_waveform = file.read(length)
            if not byte_waveform: break
            yield numpy.fromiter(array.array('b', byte_waveform), dtype=numpy.int)
            frame_read += 1
    finally:
        file.close()



class average:
    """accumulator object for getting mean of a set fo waveform"""
    
    def from_collection(waveform_collection):
        mean = average()
        for waveform in waveform_collection: mean.add(waveform)
        return mean.compute()

    def __init__(me):
        me.mean = None
        me.waveforms = 0

    def add(me, waveform):
        if me.mean is None: 
            me.mean = numpy.linspace(0, 0, len(waveform))
        me.mean += waveform
        me.waveforms += 1

    def compute(me):
        return me.mean / me.waveforms



class spectrum:
    """accumulator object to get mean spectrum of a set of waveforms"""

    def from_collection(waveform_collection):
        spec = spectrum()
        for waveform in waveform_collection: spec.add(waveform)
        return spec.compute()

    def power(waveform):
        fft = numpy.fft.rfft(waveform)
        return numpy.real(fft * numpy.conjugate(fft))

    def __init__(me):
        me.components = average()

    def add(me, waveform):
        me.components.add(spectrum.power(waveform))

    def compute(me):
        return 10 * numpy.log10(me.components.compute())
        


# TODO(hugo) check if slice_at is uselesss or no
class slice_at:
    """glorified array to clean histogram preparation"""
    
    def from_collection(index, waveform_collection):    
        return [waveform[index] for waveform in waveform_collection]

    def __init__(me, index):
        me.at = index
        me.values = []

    def add(me, waveform):
        me.values.append(waveform[me.at])



class trace:
    """
    compute a matrix which is a stacked image of all waveforms in a set.
    ti improve dynamic range, every column is normalized independently using its maximum value
    """
    
    def from_collection(waveform_collection):
        trace_mat = trace()
        for waveform in waveform_collection: trace_mat.add(waveform)
        return trace.compute()

    def __init__(me):
        me.trace = None

    def add(me, waveform):
        if me.trace is None:
            me.trace = numpy.zeros((len(waveform), 256), dtype=numpy.dtype('d'))
        for axis, height in enumerate(waveform):
      # the y axis is reversed by default in pylab.imshow()
            # therefore 128-height gives the correct y-axis points
            # also, 128-height is implicity converted to math.floor(128-height) apparently
            me.trace[axis][128-height] += 1.0   # the y axis is reversed in the pylab.imshow() routine

    def compute(me):
        """
        normalization by column to optimize dynamic range and local contrast
        """
        for col in me.trace:
            max_of_col = max(col)
            for heigth, pixel in enumerate(col):
                col[heigth] /= max_of_col
        return me.trace


# TODO(hugo) for the four above class write with reduce function (using generator reuse)
