package radix

import (
	"fmt"
)

func main() {
	in := []int32{2, 4, 657, 12, -10, 12, 34, 1239, -5, -2394}
	rad32(in)
	fmt.Println(in)
}

func rad32(in []int32) {
	var (
		counts [1024]int
		offset = [4]int{0, 256, 512, 768}
	)

	for _, x := range in {
		for _, o := range offset {
			radix := int(x&0xff) + o
			counts[radix]++
			x = x >> 8
		}
	}

	for _, o := range offset {
		tot := 0
		for j := 0; j < 256; j++ {
			counts[j+o], tot = tot, tot+counts[j+o]
		}
	}

	swap := make([]int32, len(in))
	for _, o := range offset {
		shift := uint(o >> 5)
		for _, x := range in {
			radix := int((x>>shift)&0xff) + o
			swap[counts[radix]] = x
			counts[radix]++
		}
		in, swap = swap, in
	}
}
