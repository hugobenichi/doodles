#!/usr/local/bin/ruby

require 'date'

location = File.open(ENV["HOME"] + "/Dropbox/.todorc", 'r').readlines.map(&:chomp).join ""

Kernel.exec 'gedit "%s"' % (location)
