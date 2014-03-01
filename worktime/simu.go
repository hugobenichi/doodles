package main

import (
  "fmt"
  "math/rand"
  "sort"
  "time"
)

/* generates a list of arrival times from given distribution */
func arrivals(days, avrg int, dist func() int) []int {
	times := make([]int, days)
  for i, _ := range times {
		times[i] = avrg + dist()
	}
	return times
}

/* cuts a fixed total size into n segments with random length */
func segments(n, tot int, dist func() int) []int {
	segs := make([]int, n)
	for i := 0; i + 1 < n; i++ {
    new_seg := ensure_pos(tot / (n - i), dist)
    segs[i] = new_seg
    tot = tot - new_seg
	}
  segs[n-1] = tot
	return segs
}

func ensure_pos(offset int, dist func() int) int {
  x := -1
  for tries := 100; x <= 0 && tries > 0; tries-- {
    x = offset + dist()
  }
  if x < 0 { x = 0 }
  return x
}

/* combines two lists of working time trying to match departure time
 * by sorting both lists in reverse order.
 * also converts to (time,min) format and shuffle the pairs. */
func pair_of(ins, segs []int, conv func(int) Time) []WorkDay {
  sort.Ints(ins)
  sort.Ints(segs)
  wdays := make([]WorkDay, len(ins))
  for i, _ := range wdays {
    from := conv(ins[i])
    to := conv(ins[i] + segs[len(segs)-1-i])
    wdays[i] = WorkDay{from, to}
  }
  for i := len(wdays) - 1; i > 1; i-- {
    j := rand.Intn(i+1)
    d := wdays[i]
    wdays[i] = wdays[j]
    wdays[j] = d
  }
	return wdays
}

type SimConf struct {
  days int
  from Time
  to Time
  sigma float64
  unit int
}

func (cfg SimConf) TotalWorkUnit() int {
	f, _ := converters(cfg.unit)
  return cfg.days * (f(cfg.to) - f(cfg.from))
}

func (cfg SimConf) TotalWorkHour() int {
	_, b := converters(cfg.unit)
  return b(cfg.TotalWorkUnit()).hour
}

func (cfg SimConf) WorkDays() []WorkDay {
	f, b := converters(cfg.unit)
	gauss := gaussian_sampler(cfg.sigma)
	t1 := arrivals(cfg.days, f(cfg.from), gauss)
  t2 := segments(cfg.days, cfg.TotalWorkUnit(), gauss)
  return pair_of(t1, t2, b)
}

func (cfg SimConf) Info() []string {
  daysInf := fmt.Sprintf("total days: %d", cfg.days)
  workhoursInf := fmt.Sprintf("total work hours: %d", cfg.TotalWorkHour())
  fromInf := fmt.Sprintf("standard arrival time: %s", cfg.from)
  toInf := fmt.Sprintf("standard leave time: %s", cfg.to)
  return append([]string{}, daysInf, workhoursInf, fromInf, toInf)
}

func (cfg SimConf) Simulate() ([]string, []WorkDay) {
  return cfg.Info(), cfg.WorkDays()
}

func main() {

  rand.Seed(time.Now().UnixNano())

	days := 20
	from := Time{9, 30}
	to := Time{18, 30}

  unit := 15
	sigma := 3.0

  cfg := SimConf{days, from, to, sigma, unit}

	for _, d := range cfg.WorkDays() {
		fmt.Println(d)
	}

}
