#!/usr/bin/python3


import re
import sys


sep = re.compile('\s+')

words = set()

with open(sys.argv[1], 'r') as file:
    voisins = ( sep.split(line) for line in file)
    for v in voisins:
        words.add(v[0])

for w in sorted([w for w in words]):
    print(w)
