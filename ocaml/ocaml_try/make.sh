#/bin/bash

ocamlfind ocamlc -linkpkg -thread -package core -o tos tos.ml
