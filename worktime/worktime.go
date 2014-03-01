package main

import (
	"fmt"
	"net/http"
	"strconv"
)

func get_days(r *http.Request) int {
	days, err := strconv.ParseInt(r.Form.Get("days"), 10, 8)
	if err != nil || days < 0 {
		days = 20
	}
	return int(days)
}

func handler(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()

	days := get_days(r)
	from := Time{9, 30}
	to := Time{18, 30}

	unit := 15
	sigma := 3.0

	cfg := SimConf{days, from, to, sigma, unit}

	info, wdays := cfg.Simulate()

	for _, s := range info {
		fmt.Fprintln(w, s)
	}

	for _, d := range wdays {
		fmt.Fprintln(w, d)
	}

}

func init() {
	http.HandleFunc("/sim", handler)
}
