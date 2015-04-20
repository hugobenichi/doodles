package secure

import (
	"crypto/sha256"
	"hash"
	"unsafe"
)

const (
	inner = sha256.Size // = 32
	outer = Seed        // = 24
)

// byteGen uses a hash.Hash to populate a byte array in a deterministic way so that
// two remote peers can generate the same sequence of bytes from a shared initial
// seed. Allocations are reduced by using an array whose size match the hashing
// function block size,
type byteGen struct {
	h     hash.Hash
	bytes [inner]byte
}

// newByteGen initializes a byteGen struct and returns a pointer to it.
func newByteGen() *byteGen {
	return &byteGen{
		h: sha256.New(),
	}
}

// NextBytes updates the internal byte array with the next sequence of bytes by
// first feeding the current content to the hash function and then filling it
// from the hash function result. It returns an address to the array.
func (n *byteGen) NextBytes() *[outer]byte {
	n.h.Write(n.bytes[:])
	n.h.Sum(n.bytes[:0])
	return subArray(&n.bytes)
}

// Seed changes the state of the internal hash.Hash by writing the given byte
// slice to it.
func (n *byteGen) Seed(seed []byte) {
	copy(n.bytes[:], seed)
}

// subArray is an helper method to cast the internal byte array to a shorter
// length array. It is safe since outer < inner. In practice the input and
// output pointer values are equal.
func subArray(p *[inner]byte) *[outer]byte {
	return (*[outer]byte)(unsafe.Pointer(p))
}
