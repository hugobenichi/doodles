#!/bin/sh

string="foo   (bar ( new Point(x, graph.getY()) ));"

echo $string

replacement_string="s|[ ]*\([()]\)[ ]*| \1 |g"

echo $string | sed "$replacement_string"
