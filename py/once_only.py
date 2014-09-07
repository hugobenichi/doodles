#!/usr/bin/env python3


# TODO: test memory consumption
def delay(func, args, kwargs):
    not_yet = object()                                  # used to flag the function as not yet run
    result, given_args, given_kwargs = not_yet, args, kwargs
    def get():
        nonlocal result, given_args, given_kwargs       # otherwise the closure does not work
        if result == not_yet:                           # if true, not yet run
            result = func(*given_args, **given_kwargs)  # get result
            given_args, given_kwargs = None, None       # clean ref to args for GC
        return result                                   # return result
    return get                                          # return the wrapper to result


# used for decorating function in conjunction with delay
def once_only(func):
    def delayed_func(*args, **kwargs): return delay(func, args, kwargs)
    return delayed_func


# TODO: write a real test
def test_once_only():

    @once_only
    def foo(string):
        print( "from foo:", string)
        return "foo"

    f = foo("hello")
    print("calling foo:")
    print(f())
    print("recalling foo:")
    print(f())

    def bar(string, x=None):
        print( "from bar:", string)
        if x is not None: print("also got x =", x)
        return "bar"

    b = once_only(bar)("hello", "world")
    print("calling bar:")
    print(b())
    print("recalling bar:")
    print(b())


test_once_only()
