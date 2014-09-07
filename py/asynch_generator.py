import functools
import Queue
import threading


def async_prefetch_wrapper(generator, buffer=10):
    """ wraps an iterater such that it produces items in the background
    uses a bounded queue to limit memory consumption"""

    def enqueue(channel, collection, end):
        for item in collection: channel.put(item)   # <- input data here
        channel.put(end)                            # finish with "end" object

    def dequeue(channel, end):
        while True:
            item = channel.get()
            if item == end: return
            yield item

    done = object()                 # this object serves as to notify the receivers of the end 
    queue = Queue.Queue(buffer)     # this queue will buffers objects between threads
    thread = threading.Thread(      # launch a thread to fetch the items in the background
        target=enqueue, 
        args=(queue, generator, done))
    thread.daemon = True
    thread.start()
    dequeue(queue, done)            #launch the dequeuing operation
    """
    # generators which pulls of the item from the queue
    while True:
        item = queue.get()
        while True: 
            item = queue.get()
            if item == done: return
            yield item
    """

def async_prefetch(func):
    """decorator to make generator functions fetch items in the background"""
    @functools.wraps(func)
    def wrapper(*args, **kwds):
        return async_prefetch_wrapper( func(*args, **kwds) )
    return wrapper
