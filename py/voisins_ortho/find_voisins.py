#!/usr/bin/python3


h = """
this script looks through a list of words and find all sets of neighbours 
where two words are neighbours if they have only one different letter
with the same order among other letters. For example: wand and ward are 
neighbours.

the algorithm generates "signatures" for every words and use these
signatures to index words in a hash map. Then the hash map contains all
sets of words sharing the same signature. 

the signature must be the same for neighbours. A signature of a word is
simply the word itself where one of the character has been replaced by
a special character not appearing in the alphabet.

ps: this script works with python3 and ascii formatted input files
"""


import re
import sys


def signatures_of(word):
    def sign_word(w, i):
        s = bytearray(w, 'ascii')
        s[i] = 33 
        return s.decode('ascii')
    return [ sign_word(word, i) for i in range(len(word))]


def index_in_dict(word, dic):
    for sign in signatures_of(word):
        if dic.get(sign) is None: dic[sign] = set()
        dic[sign].add(word)


def process_set_of_words(words):
    dic = dict()
    for w in words: index_in_dict(w, dic)
    return dic


with open(sys.argv[1], 'r') as file:
    sep = re.compile('\s+')
    with_len = lambda w: (w, len(w))
    all_words = [ with_len(sep.split(line)[0]) for line in file]

words = [ set() for i in range( max( (l for w, l in all_words) ) ) ]

for w, l in all_words: words[l-1].add(w) # words allow to remove duplicates

voisins = [ process_set_of_words(s) for s in words]


def filter_voisins(voiz):
    def dic2ary(dic):
        def set2ary(word_set):
            ary = [w for w in word_set]
            ary.sort()
            return ary
        vlist = [set2ary(vz) for vz in dic.values() if len(vz) > 1 ]
        vlist.sort(key=lambda word_ary: "".join(word_ary))
        return vlist
    for i, at_i in enumerate(voiz): voiz[i] = dic2ary(at_i)

filter_voisins(voisins)


if False:
    for vl in voisins:
        for v in vl:
            print(v)


for v in voisins[5]: print(" ".join(v))


