#!/bin/bash

gcc -c -g -o heap.o heap.c -std=gnu99
gcc -c -g -o heap_test.o heap_test.c -std=gnu99
gcc -o heap_test heap.o heap_test.o
