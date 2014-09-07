#!/usr/bin/python3

import random

days = ["monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"]

bound = len(days) -1

times = 10

generator = random.Random()  # prepare generateur de nbrs aleatoires


for _ in range(times):
    rand_index = generator.randint(0,bound)   # attention bound est inclus 
    print( days[rand_index] )
