#!/bin/bash

version=`cat VERSION`

echo "changing version number to "$version" in ..."


for source_file in src/streams/*.java
do
    echo $source_file
    sed -i "s|[0-9]\.[0-9]\.[0-9]|$version|" $source_file
done

