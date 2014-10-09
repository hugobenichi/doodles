package main

import "fmt"

func main() {

  start := make(chan interface{})

  var value []int

  for i := 0; i < 20; i++ { value = append(value, i) }

  go func() {
    fmt.Println("started")
    <-start
    for _, x := range value { fmt.Println(x) }
  }()

  //start <- "whatever"
}
