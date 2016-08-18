package cmd2

import (
	"fmt"
	"os"
)

type Dispatchable interface {
	implement_dispatchable()
}

type Select struct {
	Actions map[string]Dispatchable
}

type Action struct {
	Fn    func([]string)
	Arity int
}

func (s Select) implement_dispatchable() {}
func (a Action) implement_dispatchable() {}

func Dispatch(d Dispatchable, args []string) func() {
	//fmt.Printf("DEBUG: dispatching %v with %v\n", d, args)
	switch d := d.(type) {
	case Select:
		exit_if(len(args) == 0, "No argument")
		arg, next_args := args[0], args[1:]
		next, ok := d.Actions[arg]
		exit_if(!ok, fmt.Sprintf("Unknown command %s", arg))
		return Dispatch(next, next_args)
	case Action:
		exit_if(len(args) < d.Arity, fmt.Sprintf("Not enough arguments: want %s, got %s", d.Arity, len(args)))
		return func() {
			d.Fn(args[:d.Arity])
		}
	default:
		exit_if(true, fmt.Sprintf("Unknown type %v", d))
		return func() {}
	}
}

func ActionWithoutArgs(fn func()) Action {
	return Action{
		Fn:    func(unused []string) { fn() },
		Arity: 0,
	}
}

func exit_if(cond bool, msg string) {
	if !cond {
		return
	}
	fmt.Fprintln(os.Stderr, msg)
	os.Exit(1)
}
