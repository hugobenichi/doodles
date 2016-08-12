package main

import (
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"
	"time"
)

/*
	status:
		list, scan, copy works
		issue with modtime lost when copying files
	next steps
		define a Config struct for propagating constants
		create test populating function
		write integration tests
*/

// TODO: introduce config struct
var (
	// TEST
	source_root  = "/tmp/fotoq/test/in"
	staging_root = "/tmp/fotoq/test/out"

	// PROD
	//source_root  = "/Volumes/Untitled/DCIM/"
	//staging_root = "/Users/hugobenichi/Desktop/photos/staging"

	exts = []string{
		"JPG",
		"RAF",
	}
	dir_of_ext = map[string]string{
		"RAF": "/raw",
	}
)

func main() {
	//cmd_list()
	//cmd_scan()
	cmd_copy()
}

func cmd_list() {
	for _, inf := range import_list(source_root, exts) {
		fmt.Printf("%s -> %s\n", inf.item, inf.staging)
	}
}

func cmd_scan() {
	by_staging := group_by_staging(import_list(source_root, exts))
	for _, k := range sorted_keys_of(by_staging) {
		fmt.Println(k)
		for _, inf := range by_staging[k] {
			fmt.Printf("  %s -> %s\n", inf.item, inf.new_path())
		}
	}
}

func cmd_copy() {
	by_staging := group_by_staging(import_list(source_root, exts))
	for _, k := range sorted_keys_of(by_staging) {
		fmt.Println(k)
		die_if(os.MkdirAll(k, os.ModePerm))
		for _, inf := range by_staging[k] {
			fmt.Printf("  %s -> %s\n", inf.item, inf.new_path())
			die_if(os.Rename(inf.item, inf.new_path()))
		}
	}
}

type ItemInfo struct {
	item    string
	ext     string
	staging string
	modtime time.Time
}

func info_of(item string) ItemInfo {
	info, err := os.Stat(item)
	die_if(err)
	t := info.ModTime() // TODO: try to use creation time ?
	e := suffix_of(item, exts)
	return ItemInfo{
		item:    item,
		ext:     e,
		staging: staging_dir_of(staging_root, t, e),
		modtime: t,
	}
}

func (inf *ItemInfo) new_path() string {
	return inf.staging + "/" + filepath.Base(inf.item)
}

func import_list(source_root string, exts []string) []ItemInfo {
	items := []ItemInfo{}
	for _, e := range exts {
		ms, err := filepath.Glob(pattern_of(source_root, e))
		die_if(err)
		for _, m := range ms {
			items = append(items, info_of(m))
		}
	}
	return items
}

func pattern_of(root string, ext string) string {
	return root + "/*/*." + ext
}

func suffix_of(item string, exts []string) string {
	for _, e := range exts {
		if strings.HasSuffix(item, e) {
			return e
		}
	}
	panic(fmt.Errorf("%s has no suffix in %v", item, exts))
}

func staging_dir_of(staging_root string, t time.Time, ext string) string {
	// TODO: apply timezone shift
	return staging_root + t.Format("/2006-01-02") + dir_of_ext[ext]
}

func group_by_staging(items []ItemInfo) map[string][]ItemInfo {
	m := make(map[string][]ItemInfo)
	for _, inf := range items {
		m[inf.staging] = append(m[inf.staging], inf)
	}
	return m
}

func sorted_keys_of(m map[string][]ItemInfo) []string {
	ks := []string{}
	for k := range m {
		ks = append(ks, k)
	}
	sort.Strings(ks)
	return ks
}

func die_if(err error) {
	if err != nil {
		panic(err)
	}
}
