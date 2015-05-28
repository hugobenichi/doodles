#!/bin/bash

#set -eux

targ=$1
jpge="JPG"
rawe="RAF"

function has_jpg {
  while read f; do
    jpg=$(echo $f | sed "s|$rawe|$jpge|" | sed "s|raw/||")
    [ -e $jpg ] || echo $f
  done
}

find $targ/raw -type f | has_jpg | xargs -n 1 rm
