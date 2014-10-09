package main

import (
  "fmt"
  "tos.org/doublr"
)

func Double(s string) string {
    s += s
    return s
}

func main() {

  fmt.Println(doublr.Double("hello world"))

}
