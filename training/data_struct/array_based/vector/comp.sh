#!/bin/bash

gcc -c -g -o vector.o vector.c -std=gnu99
gcc -c -g -o vector_test.o vector_test.c -std=gnu99
gcc -o vector_test vector.o vector_test.o 
