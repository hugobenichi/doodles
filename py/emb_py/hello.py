#!/usr/bin/env python3

print("hello from Python", 2+2)


#import sys
#sys.path.append("/usr/share/pyshared/numpy")
#print(sys.path)

#print("importing numpy")
#import numpy


def bar():
    print("bar")

def foo(number):
    print("foo got", number)
    return 42


def print_ary(ary):
    print(type(ary))
    for x in ary: print(x)


print("end of py script")
