package main

import "fmt"

// substring search. Search match from end of string, skip ahead when no match.
func search(pattern, text string) int {
	n := len(pattern)
	m := len(text)
	i := n - 1
	c := pattern[i]
	for {
		for i < m && c != text[i] {
			i++
		}
		if i >= m {
			return -1
		}
		j := i - 1
		k := n - 2
		all := true
		any := false
		for all && k >= 0 {
			all = (text[j] == pattern[k])
			if !any && all {
				any = true
				i += n - k
			}
			j--
			k--
		}
		if all {
			return j + 1
		}
		i++
	}
	return -1
}

func main() {
	fmt.Println(search("foo", "anpanman"))
	fmt.Println(search("pan", "blobloblobloanpanman"))
	fmt.Println(search("pan", "pan"))
}
