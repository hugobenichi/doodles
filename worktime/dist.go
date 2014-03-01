package main

import (
	"math"
	"math/rand"
)

/* returns a gaussian distribution sampler as a callable function */
func gaussian_sampler(sigma float64) func() int {
	return func() int {
    return int(sigma * rand.NormFloat64())
	}
}

/* returns a poisson distribution sampler as a callable function */
func poisson_sampler(mean float64) func() int {
	return func() int {
		n := 0
		prob := math.Exp(-mean)             // p(n==0)
		cumul := rand.Float64() - prob      // start in [0.0, 1.0[
		for cumul > 0.0 && n != 100 {
			n++                               // order of statement matters
			prob = prob * mean / float64(n)   // Poiss prob of next n value
			cumul = cumul - prob
		}
		return n
	}
}
