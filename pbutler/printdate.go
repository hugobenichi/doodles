package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	r := bufio.NewReader(os.Stdin)
	for {
		line, err := r.ReadString('\n')
		if err != nil { // EOF
			return
		}
		info := infoOf(line[:len(line)-1])
		fmt.Println(formatInfo(info))
	}
}

func infoOf(path string) os.FileInfo {
	info, err := os.Lstat(path)
	if err != err {
		panic(err)
	}
	return info
}

func formatInfo(info os.FileInfo) string {
	y := info.ModTime().Year()
	m := info.ModTime().Month()
	d := info.ModTime().Day()
	return fmt.Sprintf("%d-%02d-%02d %q", y, int(m), d, info.Name())
}
