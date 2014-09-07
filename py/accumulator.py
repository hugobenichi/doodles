#!/usr/bin/env python3


def transform(**kwargs):
    todo = (kwargs.get(k) for k in ("map", "reduce", "inject", "finalize"))
    (mapop, redop, inject, finalize) = todo
    if mapop is None and redop is None: return (None, None)
    if mapop is None: mapop = lambda x:x
    if redop is None:       # when redop is None, we ignore inject
        inject = []         # and assume the caller just want a map operation
        def redop(x, y):    # so we fold the list into an array after mapping
            x.append(y)
            return x

    def get_result():       # necessary when inject is an immediate value
        nonlocal inject     # otherwise only initial value is returned
        return inject

    def consume():
        nonlocal inject
        try:
            if inject is None: inject = mapop((yield))
            while True: inject = redop(inject, mapop((yield)))
        except GeneratorExit:
            if finalize is not None: inject = finalize(inject)

    consumer = consume()    # create the coroutine
    next(consumer)          # and initialize it
    return (get_result, consumer)


class timestop:

    def __init__(self, collection):
        self.all_receivers = []         # stocks all channels to send data into
        self.not_ran_yet = True         # just a flag to trigger once only
        self.collection = collection    # ref to the collection we are wrapping

    def todo(self, **kwargs):
        if not self.not_ran_yet: return None    # forbids to rerun the thing
        (result, receiver) = transform(**kwargs)
        self.all_receivers.append(receiver)
        def future():
            if self.not_ran_yet: self.trigger()
            return result()
        return future   # the client get this. When it is called, the comp is run

    def trigger(self):
        self.not_ran_yet = False
        for item in self.collection:
            for receiver in self.all_receivers: receiver.send(item)
        for receiver in self.all_receivers: receiver.close()


#################
#   test zone   #
#################


def test_timestop():
    def enum_col(col):
        for x in col:
            print(x)
            yield(x)

    a = (x for x in range(10))
    b = enum_col(a)

    s = timestop(b)

    x = s.todo(map=lambda x: 2*x)  # should return [0, 2, 4, 6, ..., 18]
    y = s.todo(reduce=lambda x, y: x+y) # should return 45
    z = s.todo(map=lambda x: 2*x, reduce=lambda x, y: x+y) # should return 90

    print("before trigger")

    print("calling x")
    print(x())
    print("calling y")
    print(z())
    print("calling y")
    print(y())


def test_a_transform(**kwargs):
    result, receiver = transform(**kwargs)
    for x in range(10): receiver.send(x)
    receiver.close()
    print(result())
    return result()


def test_all_transform():
    test_a_transform(map=lambda x: 2*x)  # should return [0, 2, 4, 6, ..., 18]
    test_a_transform(reduce=lambda x, y: x+y) # should return 45
    test_a_transform(map=lambda x: 2*x, reduce=lambda x, y: x+y) # should return 90
    def index(d,v):
        d[v[0]] = v[1]
        return d
    test_a_transform(map=lambda x: (str(x), 2*x), reduce=index, inject=dict())


#test_all_transform()
test_timestop()


