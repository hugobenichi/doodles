package secure

import (
	"encoding/binary"
	"io"

	"golang.org/x/crypto/nacl/box"
)

// Coder wraps an io.Writer with encryption. It behaves in a datagram fashion
// and writes one message at a time by cutting into fragments of fixed size.
// TODO keep this comment ?
type Coder struct {
	writer   io.Writer // the underlying source of data.
	buffer   []byte    // used to collect encrypted blocks.
	block    []byte    // used for first and last fragments of a message.
	*byteGen           // a source of random bytes for nonce.
	shared   [Key]byte // decryption key.
}

// NewCoder returns a new Coder wrapping an io.Writer with encryption.
func NewCoder(w io.Writer, priv, pub *[Key]byte) *Coder {
	c := &Coder{
		writer:  w,
		buffer:  make([]byte, 0, Buffer),
		block:   make([]byte, Block),
		byteGen: newByteGen(),
	}
	box.Precompute(&c.shared, pub, priv)
	return c
}

// TODO comments
// Write version in which the input msg in cut in 1024 slices, encoded, and then
// pushed in one go to the underlying writer.
// we don't encode one big block for small messages case
func (c *Coder) Write(p []byte) (int, error) {
	var (
		out   = c.buffer
		block = c.block
		tail  = firstBlock(c.block, p)
	)

	for len(block) != 0 {
		out = box.SealAfterPrecomputation(out, block, c.NextBytes(), &c.shared)
		block, tail = nextBlock(block, tail)
	}

	if _, err := c.writer.Write(out); err != nil {
		return 0, err // return 0 ?? calculate from underlying Write return ??
	}
	return len(p), nil
}

// firstBlock writes the length of p in block as an uint32 in network order
// and then copies the first 1020 bytes of p in block. It returns the next
// fragment of p or nil if p is smaller than 1020 bytes.
func firstBlock(block, p []byte) []byte {
	l := len(p)
	binary.BigEndian.PutUint32(block, uint32(l))
	copy(block[4:], p)
	if l > Block-4 {
		return p[Block-4:]
	}
	return nil
}

// nextBlock returns the next block to encrypt and the remaining data after
// that block.
func nextBlock(block, p []byte) ([]byte, []byte) {
	l := len(p)
	switch {
	case l >= Block:
		return p[:Block], p[Block:]
	case l == 0:
		return nil, nil
	}
	copy(block, p)
	return block, nil
}
