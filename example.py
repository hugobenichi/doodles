#!/usr/bin/env python


import game_of_life


state = ( (0,0), (1,0), (2,0), (2,1), (1,2) )
state = ( (0,1), (1,0), (0,0), (1,1) )
state = ( (0,1), (1,0), (2,0), (3,1), (1,2), (2,2) )
state = ( (0,0), (-1,0), (+1,0) )


for i in range(8):
    state = game_of_life.update(state)
    print(" ".join(map(str, state)))

