package main

import (
	"fmt"
)

type mat [4]int

var (
	zero = mat([4]int{0, 0, 0, 0})
	one  = mat([4]int{1, 0, 0, 1})
)

func (a *mat) mul(b *mat) mat {
	var c mat
	c[0] = a[0]*b[0] + a[1]*b[2]
	c[1] = a[0]*b[1] + a[1]*b[3]
	c[2] = a[2]*b[0] + a[3]*b[2]
	c[3] = a[2]*b[1] + a[3]*b[3]
	return c
}

func (a *mat) exp(n int) mat {
	switch {
	case n == 0:
		return one
	case n == 1:
		return *a
	case n&1 == 1:
		b := a.exp(n - 1)
		return a.mul(&b)
	default:
		b := a.exp(n / 2)
		return b.mul(&b)
	}
}

var fib1 = mat([4]int{0, 1, 1, 1})

func fib(n int) int {
	return fib1.exp(n)[3]
}

func main() {
	for i := 0; i < 15; i++ {
		fmt.Println(fib1.exp(i), "   ", fib(i))
	}
}
