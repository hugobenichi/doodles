package main

import (
	"./importing"
	"./selecting"
)

func main() {
	//env := "test"
	env := "prod"
	cmd := importing.Scan // Copy

	c := cfg[env]
	c.Do(cmd)

	c2 := selecting.Config{}
	c2.Do(selecting.Diff)
}

var (
	cfg = map[string]importing.Config{
		"test": {
			Source: "/tmp/fotoq/test/in",
			Output: "/tmp/fotoq/test/out",
			Exts: []string{
				"JPG",
				"RAF",
			},
			DirOfExt: map[string]string{
				"RAF": "/raw",
			},
		},
		"prod": {
			Source: "/Volumes/Untitled/DCIM/",
			Output: "/Users/hugobenichi/Desktop/photos/staging",
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
