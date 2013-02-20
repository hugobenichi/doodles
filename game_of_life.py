#!/usr/bin/env python


import itertools
import functools


def survive(point, neighbors, is_alive):
    return neighbors == 3 or (neighbors == 2 and is_alive)


def translate(iterable, offset):
    a, b = offset
    for (x,y) in iterable: yield (x-a, y-b)


def translate_all(iterable):
    offsets = [ (x,y) for x in (-1,0,+1) for y in (-1,0,+1) if x != 0 or y != 0]
    return iter(itertools.chain( *[ translate(iterable, p) for p in offsets ] ))


def update_count(dic, point):
    if dic.get(point) is None: dic[point] = 0
    dic[point] += 1
    return dic


def update(alive):
    counts = functools.reduce(update_count, translate_all(alive), dict())
    return set(( p for (p, n) in counts.items() if survive(p, n, p in alive) ))


state = ( (0,0), (1,0), (2,0), (2,1), (1,2) )
state = ( (0,1), (1,0), (0,0), (1,1) )
state = ( (0,1), (1,0), (2,0), (3,1), (1,2), (2,2) )
state = ( (0,0), (-1,0), (+1,0) )


for i in range(8):
    state = update(state)
    print(" ".join(map(str, state)))

