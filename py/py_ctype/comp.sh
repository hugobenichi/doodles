#!/bin/bash


gcc -c hw.c -fPIC -std=c99
gcc -shared -o hw.so hw.o
