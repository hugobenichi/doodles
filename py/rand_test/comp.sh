#!/bin/bash

gcc -c rand_call.c -fPIC -std=gnu99
gcc -shared -o rand_call.so rand_call.o

