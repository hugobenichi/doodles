// a Go translation of http://xorshift.di.unimi.it/
// TODO:		implement xorgens4096 ?
//					implement MT19937-64 ?
//					add jump functions
//      		pass them through http://simul.iro.umontreal.ca/testu01/tu01.html
package main

import "fmt"

// in prng package, simply call it prng.UInt64
type UInt64 interface {
	Next() uint64
}

type Float64 interface {
	Next() float64
}

var (
	f = 1. / (1 << 52)
	m = uint64(1<<52) - 1
)

// DoubleOf generates a double in [0,1[ from the 52 low bits of an uint64.
func DoubleOf(u uint64) float64 {
	return float64(u&m) * f // has form n * 2 ^ -52 with n a 52 bits unsigned int.
}

/* --  xorshift128+  -------------------------------------------------------- */

// The xorshift128+ PRNG. Public fields for inspection and custom seeding.
type XS128P struct {
	S0, S1 uint64
}

var xs128p_0 = XS128P{S0: 1, S1: 2}

func NewXS128P() XS128P { return xs128p_0 }

func (r *XS128P) Next() uint64 {
	s1, s0 := r.S0, r.S1 // swap intended

	s1 ^= s1 << 23
	s1 ^= s1 >> 17
	s1 ^= s0
	s1 ^= s0 >> 26

	r.S0, r.S1 = s0, s1

	return s0 + s1
}

/* -------------------------------------------------------------------------- */

/* --  xorshift64*  --------------------------------------------------------- */

type XS64S uint64

func NewXS64S() XS64S { return 42 }

func (r *XS64S) Next() uint64 {
	u := *r

	u ^= u >> 12
	u ^= u << 25
	u ^= u >> 27

	*r = u

	return uint64(u) * 2685821657736338717
}

/* -------------------------------------------------------------------------- */

/* --  xorshift1024*  ------------------------------------------------------- */

// The xorshift1024* PRNG. Public fields allow inspection and custom seeding.
type XS1024S struct {
	State [16]uint64
	I     int
}

func NewXS1024S() XS1024S {
	return XS1024S{
		State: [16]uint64{
			// generated from XS128P after dropping first 10 outputs
			10496030469740439798,
			1362371001014398178,
			10234834343287503199,
			2065174045786219692,
			4360127029907502923,
			3280463725544326876,
			4098392981707075411,
			3999493022945510211,
			1490933961015620186,
			13252044636817621309,
			15998111192842087806,
			3805184684654049962,
			2739691196446076535,
			15675371886373393618,
			16373398947986388217,
			12742849885299357362,
		},
		I: 0,
	}
}

func (r *XS1024S) Next() uint64 {
	var (
		i  = r.I
		j  = (i + 1) & 0xF
		s0 = r.State[i]
		s1 = r.State[j]
	)

	s1 ^= s1 << 31
	s1 = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30)

	r.State[j] = s1
	r.I = j

	return s1 * 1181783497276652981
}

/* -------------------------------------------------------------------------- */

func main() {
	//r := NewXS128P()
	//r := NewXS1024S()
	r := NewXS64S()

	for i := 0; i < 64; i++ {
		//fmt.Println(r.Next())
		fmt.Println(DoubleOf(r.Next()))
	}
}
