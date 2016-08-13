package main

import (
	"./importing"
)

func main() {
	//env := "test"
	env := "prod"
	cmd := importing.Scan // Copy

	c := cfg[env]
	c.Do(cmd)
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
