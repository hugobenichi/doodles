#!/bin/bash

addline="s|$|\n|"

gcc -c -g -o object/dmatrix.o -I include src/dmatrix.c 2>&1 | sed $addline

gcc -c -g -o object/maximum_likelihood/interface.o -I include src/maximum_likelihood/interface.c 2>&1 | sed $addline

gcc -c -g -o object/point_cloud/partition.o -I include src/point_cloud/partition.c 2>&1 | sed $addline

gcc -o bin/maxlik_test object/maximum_likelihood/interface.o object/dmatrix.o 2>&1 | sed $addline


