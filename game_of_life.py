#!/usr/bin/env python

"""
Implementation of Conway's Game of Life in a functional oriented style using
python's iterators.

Points are represented by 2-tuple storing the x coordinate and y coordinate in
the board.

At every round, all alive points and only alive points are stored in a hash set
which allows O(1) access and test for presence.

The function update() find all points with 1 or more living neighbors and
compute the number of neighbors for all these points, using the 3 functions
translate(), translate_all() and update_count().

Then, update() uses this count data, presence of points in the set and calls to
survive() to generate a new set of living points for the next round.

The algorithm for computing the number of neighbors is based on moving average
filters. The set of living points is translated by one in all possible
directions and added together. The result is a grid of sums which directly tells
the number of living neighbors for every points in the board.

For example let's assume the set of living point is:
 ______
|      |
| x x  |
| xxxx |
|      |
|______|

All translated sets are:
 ______   ______   ______   ______   ______   ______   ______   ______
|x x   | | x x  | |  x x | |      | |      | |      | |      | |      |
|xxxx  | | xxxx | |  xxxx| |x x   | |  x x | |      | |      | |      |
|      | |      | |      | |xxxx  | |  xxxx| |x x   | | x x  | |  x x |
|      | |      | |      | |      | |      | |xxxx  | | xxxx | |  xxxx|
|______| |______| |______| |______| |______| |______| |______| |______|

The sum over these 8 sets is:
 ______
|11211 |
|225331|
|224321|
|123321|
|______|

The living points for next round are
 ______
|      |
| x xx |
| x xx |
|  xx  |
|______|

"""

import functools
import itertools


__author__      = "Hugo Benichi"
__copyright__   = "Copyright 2013, Hugo Benichi"
__email__       = "hugo[dot]benichi[at]m4x[dot]org"
__status__      = "toy"


def survive(point, neighbors, is_alive):
    """
    Returns true if the 2-tuple argument "point" survives this round, and false
    otherwise. "neighbors" is the number of alive neighbors next to point.
    "is_alive" is a boolean value saying if "point" was alive the round before.
    """
    return neighbors == 3 or (neighbors == 2 and is_alive)


def translate(iterable, offset):
    """
    Returns a generator of 2-tuples representing (x,y) points and built from the
    collection of 2-tuples "iterable". Each point returned are translated in x
    and y by (a,b) = offset.
    """
    a, b = offset
    for (x,y) in iterable: yield (x-a, y-b)


def translate_all(iterable):
    """
    Returns a chain of generators of all possible translations of the collection
    of 2-tuples "iterable", by +/- 1 in x and y, except the 0 translation.
    """
    offsets = [ (x,y) for x in (-1,0,+1) for y in (-1,0,+1) if x != 0 or y != 0]
    return iter(itertools.chain( *[ translate(iterable, p) for p in offsets ] ))


def update_count(dic, point):
    """
    Updates the count of appearance of the 2-tuple "point". "dic" stores the
    integer count value for the 2-tuple key. If point is found for the first
    time, the count is set to 1. Returns a reference to the dictionary, allowing
    to use update_count() with functools.reduce().
    """
    if dic.get(point) is None: dic[point] = 0
    dic[point] += 1
    return dic


def update(alives):
    """
    Updates the set of living points represented as 2-tuple in the set "alives".
    Returns a new set with surviving points and new points which came to life.

    First a dictionary of all points with living neighbors is generated from
    translated iterators over the collection of points alive on this round. The
    keys of the dictionary are the points with 1 or more neighbors. The values
    are the number of neighbors.

    The dictionary is then used to find all surviving points already alive and
    all new living points.
    """
    counts = functools.reduce(update_count, translate_all(alives), dict())
    return set(( p for (p, n) in counts.items() if survive(p, n, p in alives) ))


