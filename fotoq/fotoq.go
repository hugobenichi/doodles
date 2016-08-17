package main

import (
	"fmt"
	"os"

	"./conf"
	"./importing"
	"./selecting"
)

func main() {
	if len(os.Args) < 3 {
		fmt.Println("Need 2 arguments")
		fmt.Println("TODO: usage")
		os.Exit(1)
	}
	Do(Cmd(os.Args[1]))
}

type Cmd string

var (
	Import = Cmd("import")
	Select = Cmd("select")
	Test   = Cmd("test")
)

func Do(cmd Cmd) {
	c := cfg["test"]
	//c := cfg["prod"]
	switch cmd {
	case Import:
		importing.Do(c, importing.Cmd(os.Args[2]))
	case Select:
		selecting.Do(c, selecting.Cmd(os.Args[2]))
	case Test:
		// integration tests
	default:
		fmt.Printf("Unknown main command %s\n", cmd)
		fmt.Println("TODO: usage")
		os.Exit(1)
	}
}

var (
	cfg = map[string]conf.C{
		"test": {
			Source:  "/tmp/fotoq/test/in",
			Staging: "/tmp/fotoq/test/out",
			Exts: []string{
				"JPG",
				"RAF",
			},
			DirOfExt: map[string]string{
				"RAF": "/raw",
			},
		},
		"prod": {
			Source:  "/Volumes/Untitled/DCIM/",
			Staging: "/Users/hugobenichi/Desktop/photos/staging",
			Exts: []string{
				"JPG",
				"RAF",
			},
			DirOfExt: map[string]string{
				"RAF": "/raw",
			},
		},
	}
)
