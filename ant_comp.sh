#!/bin/bash

target=$1

if test -z "$target"
then target=compile
fi

echo $target

export JAVA_HOME=/usr/lib/jvm/java-8-oracle/
ant $target
