#!/usr/local/bin/ruby

File.open(ARGV[0]){ |input| input.readline.tap{|s| puts s if s.ascii_only?} until input.eof? }
