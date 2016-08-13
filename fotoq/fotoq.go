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
	next steps
		introduce Cmd enum for list, scan, copy
		add timezone shift in Config
		create test populating function,
			how to exercice different modtime ?
				use a parametrized function inside Config
		write integration tests
*/

func main() {
	//c := prod_cfg
	c := test_cfg

	//c.cmd_list()
	c.cmd_scan()
	//c.cmd_copy()
}

type Config struct {
	source_root  string
	staging_root string
	exts         []string
	dir_of_ext   map[string]string
}

var (
	test_cfg = Config{
		source_root:  "/tmp/fotoq/test/in",
		staging_root: "/tmp/fotoq/test/out",
		exts: []string{
			"JPG",
			"RAF",
		},
		dir_of_ext: map[string]string{
			"RAF": "/raw",
		},
	}

	prod_cfg = Config{
		source_root:  "/Volumes/Untitled/DCIM/",
		staging_root: "/Users/hugobenichi/Desktop/photos/staging",
		exts: []string{
			"JPG",
			"RAF",
		},
		dir_of_ext: map[string]string{
			"RAF": "/raw",
		},
	}
)

func (c *Config) cmd_list() {
	for _, inf := range c.import_list() {
		fmt.Printf("%s -> %s\n", inf.item, inf.staging)
	}
}

func (c *Config) cmd_scan() {
	by_staging := group_by_staging(c.import_list())
	for _, k := range sorted_keys_of(by_staging) {
		fmt.Println(k)
		for _, inf := range by_staging[k] {
			fmt.Printf("  %s -> %s\n", inf.item, inf.new_path_of())
		}
	}
}

func (c *Config) cmd_copy() {
	by_staging := group_by_staging(c.import_list())
	for _, k := range sorted_keys_of(by_staging) {
		fmt.Println(k)
		die_if(os.MkdirAll(k, os.ModePerm))
		for _, inf := range by_staging[k] {
			fmt.Printf("  %s -> %s\n", inf.item, inf.new_path_of())
			die_if(os.Rename(inf.item, inf.new_path_of()))
		}
	}
}

type ItemInfo struct {
	item    string
	ext     string
	staging string
	modtime time.Time
}

func (c *Config) info_of(item string) ItemInfo {
	info, err := os.Stat(item)
	die_if(err)
	t := info.ModTime() // TODO: try to use creation time ?
	e := c.suffix_of(item)
	return ItemInfo{
		item:    item,
		ext:     e,
		staging: c.staging_dir_of(t, e),
		modtime: t,
	}
}

func (inf *ItemInfo) new_path_of() string {
	return inf.staging + "/" + filepath.Base(inf.item)
}

func (c *Config) import_list() []ItemInfo {
	items := []ItemInfo{}
	for _, e := range c.exts {
		ms, err := filepath.Glob(c.pattern_of(e))
		die_if(err)
		for _, m := range ms {
			items = append(items, c.info_of(m))
		}
	}
	return items
}

func (c *Config) pattern_of(ext string) string {
	return c.source_root + "/*/*." + ext
}

func (c *Config) suffix_of(item string) string {
	for _, e := range c.exts {
		if strings.HasSuffix(item, e) {
			return e
		}
	}
	panic(fmt.Errorf("%s has no suffix in %v", item, c.exts))
}

func (c *Config) staging_dir_of(t time.Time, ext string) string {
	// TODO: apply timezone shift
	return c.staging_root + t.Format("/2006-01-02") + c.dir_of_ext[ext]
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
