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


import threading
import queue


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


def binary_reader(path, length):
    """generators which reads a file of binary waveforms and return one at a time."""
    try:
        file = open(path, 'rb')
        while True:
            bytes = file.read(length)
            if not bytes: break
            yield bytes
    except IOError:
        sys.stderr.write("error while opening binary file at %s\n" % path)
        sys.exit(2)
    finally:
        file.close()


def reader(path, length=1000, frame=-1, dc=0):
    """generators which converts binary waveforms to numpy arrays and returns them one at a time
    binary format: signed int8 
    the 'length' parameter controls the number of points ver waveform.
    the 'frame' parameter controls the total number of waveforms to read.
    the 'dc' parameter translates every waveform by minus this amount.
    """
    frame_read = 0
    reader = binary_reader(path, length)
    while (frame < 0 or frame > frame_read):
        for byte_waveform in reader:
            yield numpy.fromstring(byte_waveform, dtype='int8') - dc  #(signed char format)
            frame_read += 1
        break


def async_reader(path, length=1000, frame=-1, dc = 0, depth=100, pack=100):
    """doest not work very well"""
    """
    def async_binary_reader(channel, end):
        try:
            file = open(path, 'rb')
            while True:
                chunk = []
                for i in range(pack):
                    bytes = file.read(length)
                    if not bytes: 
                        channel.put(end)
                        chunk = None
                        break
                    chunk.append(bytes)
                if not chunk: break
                channel.put(chunk)
        except IOError:
            sys.stderr.write("error while opening binary file at %s\n" % path)
            sys.exit(2)
        finally:
            file.close()
    """
    def async_binary_reader(channel, end):
        file = open(path, 'rb')
        while True:
            chunk = []
            for i in range(pack):
                bytes = file.read(length)
                if not bytes: 
                    channel.put(end)
                    chunk = None
                    break
                chunk.append(bytes)
            if not chunk: break
            channel.put(chunk)
        file.close()

    done = object()                 # this object serves as to notify the receivers of the end 
    buff = queue.Queue(depth)       # this queue will buffers objects between threads
    thread = threading.Thread(      # launch a thread to fetch the items in the background
        target=async_binary_reader, 
        args=(buff, done))
    thread.start()

    frame_read = 0
    while (frame < 0 or frame > frame_read):
        byte_waveform = buff.get()
        if byte_waveform is done: break
        for bytes in byte_waveform:
            #yield numpy.fromstring(byte_waveform, dtype='int8') - dc
            yield numpy.fromstring(bytes, dtype='int8') - dc
            frame_read += 1


def async_prefetch_wrapper(generator, depth=100):
    """direclty adapted from http://niki.code-karma.com/2011/05/hiding-io-latency-in-generators-by-async-prefetching/"""
    def enqueue(channel, collection, end):
        for item in collection: channel.put(item)   # <- input data here
        channel.put(end)                            # finish with "end" object

    done = object()                 # this object serves as to notify the receivers of the end 
    buff = queue.Queue(depth)       # this queue will buffers objects between threads
    thread = threading.Thread(      # launch a thread to fetch the items in the background
        target=enqueue, 
        args=(buff, generator, done))
    thread.start()

    while True:
        item = buff.get()
        if item is done: return
        yield item


def read_binary(path, length=1000, frame=-1, dc=0):
    """deprecated. use reader instead."""
    reader(path, length, frame, dc)


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


# TODO(hugo) for the above classes write with reduce function (using generator reuse)
