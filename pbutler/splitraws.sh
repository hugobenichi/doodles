#!/bin/bash

set -eux

rawe="RAF"

targ=$1

mkdir -p $targ/raw
mv $targ/*$rawe $targ/raw/
