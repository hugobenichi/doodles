package main

import (
	"os"

	"./cmd"
	"./conf"
	"./importing"
	"./selecting"
)

func main() {
	c := cfg["prod"]
	//c := cfg["test"]
	cmd.Dispatch(Actions(c), os.Args[1:])()
}

func Actions(cfg conf.C) cmd.Dispatchable {
	return cmd.Select{
		Actions: map[string]cmd.Dispatchable{
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

var (
	cfg = map[string]conf.C{
		"test": {
			Source:  "/tmp/fotoq/test/in",
			Staging: "/tmp/fotoq/test/out",
			Working: "/tmp/fotoq/test/out",
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
			Staging: "/Volumes/Untitled/staging/",
			Working: "/Users/hugobenichi/Desktop/photos/",
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
