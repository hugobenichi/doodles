package main

import (
	"fmt"
	"os"

	"./cmd"
	"./cmd2"
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
	c := cfg["test"]

	if false {
		importing.Init(c)
		selecting.Init(c)
		Do(os.Args[1])
	}

	cmd2.Dispatch(get_dis(c), os.Args[1:])()
}

func get_dis(cfg conf.C) cmd2.Dispatchable {
	return cmd2.Select{
		Actions: map[string]cmd2.Dispatchable{
			Import: importing.Actions(cfg),
			Select: selecting.Actions(cfg),
			//Test: TODO
		},
	}
}

const (
	Import = "import"
	Select = "select"
	Test   = "test"
)

func Do(command string) {
	switch command {
	case Import:
		cmd.Dispatch(os.Args[2:])()
	case Select:
		cmd.Dispatch(os.Args[2:])()
	case Test:
		// integration tests
	default:
		fmt.Printf("Unknown main command %s\n", command)
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
