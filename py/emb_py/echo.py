#!/usr/bin/env python3


import sys


try:
    while True:
        print("echo ", sys.stdin.readline())
        print(sys.stdin.closed)
except EOFError:
    print("program finished")
