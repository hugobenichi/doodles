#!/bin/bash

javac -sourcepath src -d build/classes/ src/*/*\.java
jar cf build/jar/streams.jar -C build/classes/ .
java -jar build/jar/streams.jar
