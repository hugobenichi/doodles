package secure

import (
	"bytes"
	"crypto/rand"
	"fmt"
	"io"

	"golang.org/x/crypto/nacl/box"
)

// securedReadWriteCloser wraps an io.ReadWriteCloser with encryption and is
// created by Handshake.
type securedReadWriteCloser struct {
	io.Reader
	io.Writer
	io.Closer
}

var (
	hello      = []byte("hello:")
	helloagain = []byte("helloagain:")
)

// Handshake performs the initial exchange of public keys between two peers on
// the given io channel and returns a secured version of that channel.
// Handshake is symmetrical and does not distinguish between client and server.
//
// The protocol follows these steps:
//   1) peer A sends to peer B "hello:${pubkey_A}"
//   2) peer B asserts "hello:" prefix and reads pubkey_A
//   3) peer B sends back "hello:${pubkey_B}"
//   4) peer A asserts "hello:" prefix and reads pubkey_B
//
// The same sequence happens a second time to exchange an initial string of
// random bytes to initialize the cipher of both peers, with the difference that
// the second exchange is encrypted.
func Handshake(conn io.ReadWriteCloser) (io.ReadWriteCloser, error) {
	pubkey, privkey, err := box.GenerateKey(rand.Reader)
	if err != nil {
		return nil, err
	}

	if err := say(conn, hello, pubkey[:]); err != nil {
		return nil, err
	}

	var peerpubkey [Key]byte
	if err := listen(conn, hello, peerpubkey[:]); err != nil {
		return nil, err
	}

	secw := NewCoder(conn, privkey, &peerpubkey)
	var seed [Seed]byte
	if _, err := rand.Read(seed[:]); err != nil {
		return nil, err
	}

	if err := say(secw, helloagain, seed[:]); err != nil {
		return nil, err
	}

	secr := NewDecoder(conn, privkey, &peerpubkey)
	var peerseed [Seed]byte
	if err := listen(secr, helloagain, peerseed[:]); err != nil {
		return nil, err
	}

	secw.Seed(seed[:])
	secr.Seed(peerseed[:])

	return &securedReadWriteCloser{
		Reader: secr,
		Writer: secw,
		Closer: conn,
	}, nil
}

// say writes the given prefix and pubkey byte slices to a writer.
func say(w io.Writer, prefix []byte, pubkey []byte) error {
	_, err := w.Write(append(prefix, pubkey...))
	return err
}

// listen reads from the given io.Reader and checks that the read bytes matches
// the prefix. It then fill in the given key holder byte slice with content from
// the io.Reader.
func listen(r io.Reader, prefix []byte, keyholder []byte) error {
	buf := make([]byte, len(prefix))
	if _, err := r.Read(buf); err != nil {
		return err
	}
	if !bytes.Equal(buf, prefix) {
		return fmt.Errorf("%q dit not match expected prefix %q", buf, prefix)
	}
	n, err := r.Read(keyholder)
	if Key := len(keyholder); n != Key {
		return fmt.Errorf("could not read all %d bytes of key, only read %d", Key, n)
	}
	return err
}
