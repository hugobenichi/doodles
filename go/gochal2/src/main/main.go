// Package main implements a secured client/server communication app.
package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"

	"secure"
)

const (
	sec = true
)

// NewSecureReader instantiates a new SecureReader.
func NewSecureReader(r io.Reader, priv, pub *[32]byte) io.Reader {
	return secure.NewDecoder(r, priv, pub)
}

// NewSecureWriter instantiates a new SecureWriter.
func NewSecureWriter(w io.Writer, priv, pub *[32]byte) io.Writer {
	return secure.NewCoder(w, priv, pub)
}

// Dial generates a private/public key pair,
// connects to the server, perform the handshake
// and return a reader/writer.
func Dial(addr string) (io.ReadWriteCloser, error) {
	conn, err := net.Dial("tcp", addr)
	if err != nil {
		return nil, err
	}
	if sec {
		return secure.Handshake(conn)
	}
	return conn, err
}

// Serve starts a secured echo server on the given listener.
func Serve(l net.Listener) error {
	for {
		conn, err := l.Accept()
		if err != nil {
			return err
		}
		if sec {
			secure.Handshake(conn)
			continue
		}
		go pipe(conn, conn)
	}
	return nil
}

func secureEcho(conn io.ReadWriteCloser) {
	conn, err := secure.Handshake(conn)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	pipe(conn, conn)
}

// prompt connects an io.Reader to standard output in a new goroutine and
// connects an io.Writer to standard input in the current goroutine.
func prompt(w io.Writer, r io.Reader) {
	go pipe(os.Stdout, r)
	pipe(w, os.Stdin)
}

// pipe connects an io.Reader to an io.Writer in the current goroutine.
func pipe(w io.Writer, r io.Reader) {
	if _, err := io.Copy(w, r); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
}

func server(port int) {
	l, err := net.Listen("tcp", fmt.Sprintf(":%d", port))
	if err != nil {
		log.Fatal(err)
	}
	defer l.Close()
	log.Fatal(Serve(l))
}

func client(port int) {
	if len(os.Args) != 2 {
		log.Fatalf("Usage: %s <port>", os.Args[0])
	}
	conn, err := Dial("localhost:" + os.Args[1])
	if err != nil {
		log.Fatal(err)
	}
	prompt(conn, conn)
}

func main() {
	port := flag.Int("l", 0, "Listen mode. Specify port")
	flag.Parse()

	if *port != 0 {
		server(*port)
		return
	}

	client(*port)
}
