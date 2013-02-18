#!/bin/bash

javac -sourcepath src -d build/classes/ src/*/*\.java
jar cfm build/jar/streams.jar Manifest -C build/classes/ .
java -jar build/jar/streams.jar
