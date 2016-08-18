package cmd

import (
	"fmt"
	"os"

	"../lock"
)

type Action struct {
	Name  string
	Fn    func([]string)
	Arity int
	// TODO: add a way to consume more than zero arg. Neet to specify arrity.
}

var (
	l       lock.L
	actions map[string]Action = make(map[string]Action)
)

func Register(acts []Action) {
	defer l.Lock().Unlock()
	for _, act := range acts {
		actions[act.Name] = act
	}
}

func AsAction(name string, subactions []Action) Action {
	return Action{}
}

func Dispatch(args []string) func() { // TODO: add debug option to print dispatch chain
	defer l.Lock().Unlock()
	exit_if(len(args) == 0, "No argument") // TODO: take into account arity
	c := args[0]
	act, ok := actions[c]
	exit_if(!ok, fmt.Sprintf("Unknown command %s", c))
	exit_if(len(args) <= act.Arity, fmt.Sprintf("Not enough arguments: want %s, got %s", act.Arity, len(args)-1))
	return func() {
		act.Fn(args[1 : 1+act.Arity])
	}
}

func ZeroArity(fn func()) func([]string) {
	return func(unused []string) {
		fn()
	}
}

func ActionWithoutArgs(name string, fn func()) Action {
	return Action{
		Name: name,
		Fn:   ZeroArity(fn),
	}
}

func exit_if(cond bool, msg string) {
	if !cond {
		return
	}
	fmt.Fprintln(os.Stderr, msg)
	os.Exit(1)
}
