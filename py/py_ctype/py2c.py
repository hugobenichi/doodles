#!/usr/bin/env python3

#/usr/lib/x86_64-linux-gnu/libc.so

import ctypes

hwlib = ctypes.cdll.LoadLibrary("./hw.so")

print("\ntesting hw.hw")
hwlib.hw.argtypes = [ctypes.c_int]
hwlib.hw.restype = ctypes.c_int
print(hwlib.hw(10))


print("\ntesting hw.write")
number = ctypes.c_int(10)
print(number.value)
hwlib.write(ctypes.byref(number))
print(number.value)


print("\ntesting hw.read")

length = 9

ary = (ctypes.c_int * length)()
#ary[:] = [i*i for i in range(length)]

hwlib.read(ary, length)

print([i for i in ary])

# solution bis
#   array_type = (ctypes.c_int * length)
#   array = array_type( *[list_comprehension] )
#

# another solution
#   x = (ctypes.c_ulong*5)()
#   ctypes.cast(x, ctypes.POINTER(ctypes.c_ulong))

# Pointer to array of pointer
# ary = (ctypes.POINTER(ctypes.c_int) * length)() # pointer of pointers to int
