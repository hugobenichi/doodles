package secure

import (
	"encoding/binary"
	"errors"
	"io"

	"golang.org/x/crypto/nacl/box"
)

// Decoder wraps an io.reader and adds decryption to it.
type Decoder struct {
	reader   io.Reader // the underlying source of data.
	buffer   []byte    // used to store a full decrypted message.
	window   []byte    // a slice over the buffer to track what has been read.
	block    []byte    // used for decoding blocks from the underlying source of data.
	*byteGen           // a source of random bytes for nonce.
	shared   [Key]byte // decryption key.
}

// NewDecoder returns a new Decoder wrapping an io.Reader with decryption.
func NewDecoder(r io.Reader, priv, pub *[Key]byte) *Decoder {
	d := &Decoder{
		reader:  r,
		buffer:  make([]byte, Buffer),
		block:   make([]byte, EncryptedBlock),
		byteGen: newByteGen(),
	}
	box.Precompute(&d.shared, pub, priv)
	return d
}

func (d *Decoder) Read(p []byte) (int, error) {
	var (
		ret = len(p)
		win = d.window
	)
	for {
		p, win = transfer(p, win)
		if len(p) == 0 || len(win) != 0 {
			break
		}
		if err := d.readMessage(); err != nil {
			return 0, err
		}
		win = d.window
	}
	d.window = win
	return ret, nil
}

func transfer(dst, src []byte) ([]byte, []byte) {
	n := copy(dst, src)
	switch {
	case len(dst) > len(src):
		return dst[n:], nil
	case len(dst) < len(src):
		return nil, src[n:]
	}
	return nil, nil
}

// readMessage reads from the underlying Reader the next encrypted message
// sent by a Coder and put it in the buffer
func (d *Decoder) readMessage() error {
	var (
		out = d.buffer
		err error
	)

	if out, err = d.readBlock(out); err != nil {
		return err
	}

	l := binary.BigEndian.Uint32(d.block)
	n := numblocks(int(l)) - 1 // the remaining number of blocks to read

	for n > 0 {
		if out, err = d.readBlock(out); err != nil {
			return err
		}
		n--
	}

	d.window = d.buffer[4 : 4+l]
	return nil
}

func numblocks(l int) int {
	n := l / Block
	if l&(Block-1) != 0 {
		n++
	}
	return n
}

func (d *Decoder) readBlock(out []byte) ([]byte, error) {
	if _, err := d.reader.Read(d.block); err != nil {
		return nil, err
	}
	out, ok := box.OpenAfterPrecomputation(out, d.block, d.NextBytes(), &d.shared)
	if !ok {
		return nil, errors.New("could not authenticate encrypted block")
	}
	return out, nil
}
