package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"unsafe"

	"github.com/davecheney/profile"
)

const (
	batch = 1024 * 1024 * 16
)

func ahah() {
	defer profile.Start(profile.CPUProfile).Stop()
}

func main() {
	defer profile.Start(profile.CPUProfile).Stop()

	f, _ := os.Open("big-data")

	r := bufio.NewReaderSize(f, batch)
	var counts []float64
	for {
		provision(r) // batch reads
		bytes, err := r.ReadBytes('\n')
		if err != nil { // EOF
			fmt.Println(counts)
			return
		}
		line := unsafeString(bytes)
		counts = sum(line[:len(line)-1], counts)
	}
}

func provision(r *bufio.Reader) {
	if r.Buffered() < 1024 {
		r.Peek(batch)
	}
}

func unsafeString(b []byte) string {
	return *(*string)(unsafe.Pointer(&b))
}

func sum(line string, counts []float64) []float64 {
	j := -1
	for i := 0; j != len(line); i++ {
		line = line[j+1:]
		j = strings.IndexByte(line, ' ')
		if j == -1 {
			j = len(line)
		}
		if len(counts) <= i {
			counts = append(counts, 0)
		}
		counts[i] += atofOrZero(line[:j])
	}
	return counts
}

func atofOrZero(s string) float64 {
	f, err := strconv.ParseFloat(s, 64)
	if err != nil {
		return 0
	}
	return f
}
