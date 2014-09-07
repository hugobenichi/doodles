#!/usr/local/bin/ruby

require 'fileutils'
include FileUtils

input  = Dir.new ARGV[0]
#output = Dir.new ARGV[1]

tags = ["raw","mp2","mov",""]

def find_raw dir_path
  rez = []
  Dir.new(dir_path).reject{|e| e == "." || e == ".." }.each{ |e|
    path = "%s/%s" % [dir_path, e]
    if ["raw","mp2","mov","orig",'scan','scans','x3f'].include? e
      rez << path
    else
      rez += find_raw path if Dir.exist? path
    end
  }
  rez
end

all = find_raw(ARGV[0]).map{ |path|
  [path, ARGV[1] + path.sub(ARGV[0],"").sub(/\/([\w]+)\z/,"")]
}.each{ |old, new|
  #puts old, new
  mkdir_p new
  mv old, new
}
