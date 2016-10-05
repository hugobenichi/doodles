package selecting

import (
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"../cmd"
	"../conf"
)

type Config conf.C

const (
	Diff  = "diff"
	Clean = "clean"
)

func Actions(cfg conf.C) cmd.Dispatchable {
	c := Config(cfg)
	return cmd.Select{
		Actions: map[string]cmd.Dispatchable{
			Diff:  cmd.ActionWithoutArgs(c.cmd_diff),
			Clean: cmd.ActionWithoutArgs(c.cmd_clean),
		},
	}
}

func (c *Config) cmd_diff() {
	for _, r := range c.list_of_unpaired_raws() {
		fmt.Println(r)
	}
}

func (c *Config) cmd_clean() {
	for _, r := range c.list_of_unpaired_raws() {
		fmt.Println(r)
		die_if(os.Remove(r))
	}
}

func (c *Config) list_of_unpaired_raws() []string {
	ms := c.all_dated_directories()
	all := []string{}
	for _, m := range ms {
		all = append(all, c.unpaired_raws_at(m)...)
	}
	sort.Strings(all)
	return all
}

func (c *Config) all_dated_directories() []string {
	ms, err := filepath.Glob(c.Working + "/????-??-??*")
	die_if(err)
	return ms
}

func (c *Config) unpaired_raws_at(dir string) []string {
	// TODO: parametrize this
	jpgs := c.all_of(dir, "JPG")
	rawdir := dir + c.DirOfExt["RAF"]
	raws := c.all_of(rawdir, "RAF")
	fn := func(s string) string {
		return fmt.Sprintf("%s/%s.RAF", rawdir, s)
	}
	return map_strings(fn, subtract(raws, jpgs))
}

func (c *Config) all_of(prefix string, ext string) map[string]bool {
	ms, err := filepath.Glob(prefix + "/*." + ext)
	die_if(err)
	m := make(map[string]bool)
	for _, s := range ms {
		m[file_name(s, ext)] = true
	}
	return m
}

func file_name(path, ext string) string {
	return strings.TrimSuffix(filepath.Base(path), "."+ext)
}

func subtract(in, exclude map[string]bool) []string {
	out := []string{}
	for s := range in {
		if exclude[s] {
			continue
		}
		out = append(out, s)
	}
	return out
}

func map_strings(fn func(string) string, in []string) []string {
	out := make([]string, 0, len(in))
	for _, s := range in {
		out = append(out, fn(s))
	}
	return out
}

func die_if(err error) {
	if err != nil {
		panic(err)
	}
}
