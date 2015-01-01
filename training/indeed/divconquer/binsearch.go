package main

import "fmt"

func indexOf(target int, xs []int) int {
  from := 0
  to := len(xs)
  for from != to {
    middle := (to + from) / 2
    pivot := xs[middle]
    if pivot == target { return middle }
    if pivot < target { from = middle } else { to = middle }
  }
  return -1
}

func main() {

  data := []int{0,1,2,3,3,4,4} //,5}

  fmt.Println(indexOf(0, data))
  fmt.Println(indexOf(1, data))
  fmt.Println(indexOf(2, data))
  fmt.Println(indexOf(3, data))
  fmt.Println(indexOf(4, data))
  fmt.Println(indexOf(5, data))

  return;
}
