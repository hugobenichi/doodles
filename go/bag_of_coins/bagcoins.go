package main

import (
	"fmt"
	"math"
)

func main() {
	a := Problem{
		Sum:   100,
		Coins: []int{1, 2, 5, 10, 20},
	}.find2(0, 0)
	fmt.Println(a)
}

type Problem struct {
	Sum   int
	Coins []int
}

func (p Problem) find(t int, v int) float64 {
	if v >= p.Sum {
		return float64(t) * math.Pow(float64(len(p.Coins)), -float64(t))
	}
	a := float64(0)
	for _, c := range p.Coins {
		a += p.find(t+1, v+c)
	}
	return a
}

func (p Problem) find2(_, _ int) float64 {
	nodes := map[int]int{
		0: 1,
	}
	t := float64(0)
	b := float64(len(p.Coins))
	norm := float64(1)
	a := float64(0)
	for len(nodes) > 0 {
		next := make(map[int]int)
		for v, n := range nodes {
			if v >= p.Sum {
				a += float64(n) * t * norm
				continue
			}
			for _, c := range p.Coins {
				next[c+v] += n
			}
		}
		norm /= b
		t += 1
		nodes = next
	}
	return a
}
