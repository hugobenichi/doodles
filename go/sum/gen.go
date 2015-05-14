package main

import (
	"fmt"
	"math/rand"
)

const n = 4

func main() {
	fs := make([]interface{}, 4)
	for {
		for i := range fs {
			fs[i] = rand.Float64()
		}
		fmt.Println(fs...)
	}
}
