package importing

import (
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"
	"time"

	"../cmd"
	"../conf"
)

/*
	TODOs:
		add timezone shift in Config
		create test populating function,
			how to exercice different modtime ?
				use a parametrized function inside Config
		write integration tests
*/

type Config conf.C

type ItemInfo struct {
	item    string
	ext     string
	staging string
	modtime time.Time
}

const (
	List = "list"
	Scan = "scan"
	Copy = "copy"
)

func Actions(cfg conf.C) cmd.Dispatchable {
	c := Config(cfg)
	return cmd.Select{
		Actions: map[string]cmd.Dispatchable{
			List: cmd.ActionWithoutArgs(c.cmd_list),
			Scan: cmd.ActionWithoutArgs(c.cmd_scan),
			Copy: cmd.ActionWithoutArgs(c.cmd_copy),
		},
	}
}

func (c *Config) cmd_list() {
	for _, inf := range c.import_list() {
		fmt.Printf("%s -> %s\n", inf.item, inf.new_path_of())
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

func (c *Config) info_of(item string) ItemInfo {
	info, err := os.Stat(item)
	die_if(err)
	t := info.ModTime() // TODO: parametrize this for tests
	e := c.suffix_of(item)
	return ItemInfo{
		item:    item,
		ext:     e,
		staging: c.staging_dir_of(t, e),
		modtime: t,
	}
}

func (c *Config) import_list() []ItemInfo {
	items := []ItemInfo{}
	for _, e := range c.Exts {
		ms, err := filepath.Glob(c.pattern_of(e))
		die_if(err)
		for _, m := range ms {
			items = append(items, c.info_of(m))
		}
	}
	return items
}

func (c *Config) pattern_of(ext string) string {
	return c.Source + "/*/*." + ext
}

func (c *Config) suffix_of(item string) string {
	for _, e := range c.Exts {
		if strings.HasSuffix(item, e) {
			return e
		}
	}
	panic(fmt.Errorf("%s has no suffix in %v", item, c.Exts))
}

func (c *Config) staging_dir_of(t time.Time, ext string) string {
	// TODO: apply timezone shift
	return c.Staging + t.Format("/2006-01-02") + c.DirOfExt[ext]
}

func (inf *ItemInfo) new_path_of() string {
	return inf.staging + "/" + filepath.Base(inf.item)
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
