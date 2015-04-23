// program to prepare hashed aliases of names and websites.
//
// this is basically equivalent to the bash oneliner:
// echo "input-joined-together" | shasum -a 256 | cut -b 1-12
package main

import (
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"os"
)

func main() {
	h := sha256.New()
	for _, arg := range os.Args[1:] {
		h.Write([]byte(arg))
	}

	bytes := h.Sum(make([]byte, h.Size()))
	for bytes[0] == 0 {
		bytes = bytes[1:]
	}

	fmt.Println(hex.EncodeToString(bytes)[:12])
}
