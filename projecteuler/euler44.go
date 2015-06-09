package main

import "fmt"

type pair struct {
	j, k int64
}

// brute force approach
func main() {

	// result
	a, b := int64(1020), int64(2167)
	fmt.Println(penta(b) - penta(a))

	set := pset(100000)
	lim := int64(10000)

	candidate := []pair{}

	for i := int64(1); i < lim; i++ {
		for j := int64(1); j < lim; j++ {
			p1, p2 := penta(i), penta(i+j)
			if set[p2+p1] && set[p2-p1] {
				candidate = append(candidate, pair{i, i + j})
			}
		}
	}

	for _, c := range candidate {
		fmt.Println(c.j, c.k)
	}
}

func penta(n int64) int64 {
	return n * (3*n - 1) / 2
}

func pset(size int64) map[int64]bool {
	set := make(map[int64]bool)
	for i := int64(1); i <= size; i++ {
		set[penta(i)] = true
	}
	return set
}
