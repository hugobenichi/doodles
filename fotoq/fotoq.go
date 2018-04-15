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

const (
	sdcard = "Untitled"
	//sdcard = "NO NAME"
	//sdcard = "sd64GB_A"
	//sdcard = "sd64GB_B"
	//sdcard = "16G_A"
	//sdcard = "16GB_B"
	//sdcard = "16GB_C"
	//sdcard = "16GB_D"
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
			Source:  "/Volumes/" + sdcard + "/DCIM/",
			Staging: "/Volumes/" + sdcard + "/staging/",
			Working: "/Volumes/" + sdcard + "/staging/",
			//Working: "/Users/hugobenichi/Desktop/photos/",
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
