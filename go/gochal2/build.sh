#!/bin/bash

set -eux

unset GOROOT
unset GOPATH
export GOPATH=$(pwd)

go get golang.org/x/crypto/nacl/box
go test secure
go build main
#go test main
