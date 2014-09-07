#!/usr/bin/env python3

import sys
import random

r = random.Random()

howmany, upto = map( int, sys.argv[1:3] )

for i in range(howmany):
    print(r.randint(0, upto) )
