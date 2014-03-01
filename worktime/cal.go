package main

import "fmt"

type Time struct {
	hour, min int
}

func (t Time) String() string {
	return fmt.Sprintf("%02d:%02d", t.hour, t.min)
}

type WorkDay struct {
	arrival, leave Time
}

func (wd WorkDay) String() string {
	return fmt.Sprintf("%s\t%s", wd.arrival.String(), wd.leave.String())
}

/* pairs of functions to convert from minutes to (hour,min) and back */
func converters(unit int) (func(Time) int, func(int) Time) {
	forward := func(t Time) int {
		return (t.hour*60 + t.min) / unit
	}
	backward := func(x int) Time {
		return Time{x * unit / 60, x * unit % 60}
	}
	return forward, backward
}
