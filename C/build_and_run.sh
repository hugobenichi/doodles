#!/bin/bash

clang -msse4.2 -O2 -Wall -g maxlik.c -o maxlik && ./maxlik
