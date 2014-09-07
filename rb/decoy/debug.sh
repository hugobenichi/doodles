#!/bin/bash

VERSION=0.0.1

rake build
ruby <<EOS
  require 'decoy'
  puts Decoy.conf
EOS

tree "/usr/local/lib/ruby/gems/1.9.1/gems/decoy-"$VERSION"/"
