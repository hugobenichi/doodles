#!/usr/bin/ruby


string = "foo   (bar ( new Point(x, graph.getY()) ));"

pattern_tag = /\s*([\(\)])\s*/
pattern_cor = /\s*(<>)+\s*/

string_tagged = string.gsub pattern_tag, '<>\0<>'
string_corrct = string_tagged.gsub pattern_cor, ' '

puts string, string_tagged, string_corrct
