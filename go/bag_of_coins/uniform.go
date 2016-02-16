package main

import (
	crand "crypto/rand"
	"math/rand"
	"os"
	"strconv"

	"github.com/davecheney/profile"
)

var methods = map[string]func(int) func() int{
	"exact": uniform1,
	"crude": uniform2,
	"quasi": uniform3,
	"qfast": uniform4,
}

func main() {
	meth, ok := methods[os.Args[1]]
	if !ok {
		return
	}
	n, err := strconv.Atoi(os.Args[2])
	if err != nil {
		return
	}
	defer profile.Start(profile.CPUProfile).Stop()
	get := meth(n)
	for i := 0; i < 1e10; i++ {
		_ = get()
		//fmt.Println(get())
	}
}

func uniform1(n int) func() int {
	return func() int {
		return rand.Int() % n
	}
}

func uniform2(n int) func() int {
	var b [1]byte
	pow := 1
	for pow < n {
		pow *= 2
	}
	mask := pow - 1
	return func() int {
		crand.Read(b[:])
		i := int(b[0]) & mask
		if i >= n {
			i -= n
		}
		return i
	}
}

func uniform3(n int) func() int {
	var b [1024]byte
	index := 0
	pow := 1
	for pow < n {
		pow *= 2
	}
	mask := pow - 1
	offset := 0
	return func() int {
		if index == 1024 {
			index = 0
			crand.Read(b[:])
		}
		i := int(b[index])&mask + offset
		index++
		if i >= n {
			i -= n
		}
		if i >= n {
			i -= n
		}
		offset++
		if offset == n {
			offset = 0
		}
		return i
	}
}

func uniform4(n int) func() int {
	s := rand.NewSource(0)
	pow := int64(1)
	for pow < int64(n) {
		pow *= 2
	}
	mask := pow - 1
	offset := 0
	return func() int {
		i := int(s.Int63()&mask) + offset
		if i >= n {
			i -= n
		}
		if i >= n {
			i -= n
		}
		offset++
		if offset == n {
			offset = 0
		}
		return i
	}
}
