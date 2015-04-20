package secure

import (
	"bytes"
	"crypto/rand"
	"testing"
)

func TestByteSequencesChange(t *testing.T) {
	for i := 0; i < 10; i++ {
		gen := newByteGen()
		gen.Seed(randArray(10))
		var prev [outer]byte
		for j := 0; j < 10; j++ {
			curr := gen.NextBytes()
			if bytes.Equal(prev[:], curr[:]) {
				t.Fatalf("two successive sequences of bytes were identical: %v", prev)
			}
			copy(prev[:], curr[:])
		}
	}
}

func TestByteSequeccesAreReproducible(t *testing.T) {
	for i := 0; i < 10; i++ {
		seed := randArray(10)
		gen1 := newByteGen()
		gen2 := newByteGen()
		gen1.Seed(seed)
		gen2.Seed(seed)
		for j := 0; j < 10; j++ {
			a := gen1.NextBytes()
			b := gen2.NextBytes()
			if !bytes.Equal(a[:], b[:]) {
				t.Fatalf("sequences of bytes #%d were different: %v and %v", j+1, a, b)
			}
		}
	}
}

func randArray(l uint) []byte {
	b := make([]byte, l)
	rand.Read(b)
	return b
}
