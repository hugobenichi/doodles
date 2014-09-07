#!/usr/local/bin/ruby

require 'fileutils'
include FileUtils

input  = Dir.new ARGV[0]
#output = Dir.new ARGV[1]


def find_jpg dir_path
  rez = []
  Dir.new(dir_path).reject{|e| e == "." || e == ".." }.each{ |e|
    path = "%s/%s" % [dir_path, e]   
    rez << path if e =~ /[\w+].jpg/i
    rez += find_jpg path if Dir.exist? path
  }
  rez
end

all = find_jpg(ARGV[0]).map{ |file| 
  new =  ARGV[1] + File.dirname(file).sub(ARGV[0],"") 
  mkdir_p new
  cp file, new
}
