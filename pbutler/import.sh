#/bin/bash

set -eux

source=$1
targ=$2

find "$source" -type f \
  | ./printdate \
  | cut -d ' ' -f 1 \
  | sort | uniq \
  | xargs -I {} mkdir -p $targ/{}

find "$source" -type f \
  | ./printdate \
  | awk -v S=$source -v P=$targ '{print S"/"$2, P"/"$1"/"}' \
  | xargs -n 2 mv
