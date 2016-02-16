package main

import (
	"fmt"
	"math/rand"
)

const trials = 1e6

func main() {
	v := 100
	coins := []int{1, 2, 5, 10, 20}
	n := 0
	for i := 0; i < trials; i++ {
		n += path(v, coins)
	}
	fmt.Println(float64(n) / trials)
}

func path(v int, coins []int) int {
	n := 0
	for v > 0 {
		v -= coins[rand.Int()%len(coins)]
		n++
	}
	return n
}
