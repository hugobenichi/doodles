#!/usr/bin/env python3

import ctypes

rand = ctypes.cdll.LoadLibrary("./rand_call.so")

state1 = (ctypes.c_uint16 * 3)()
state2 = (ctypes.c_uint16 * 3)()

for i in range(3):
    state1[i] = 0
    state2[i] = 0


print( " ".join( ( "%i" % (rand.get_r(state1) % 1000) for i in range(10)) ) )


print( " ".join( ( "%i" % (rand.get_r(state2) % 1000) for i in range(10)) ) )
