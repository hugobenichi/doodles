package cmd

import (
	"fmt"
	"os"
	"sync"
)

type Action struct {
	Name string
	Fn   func()
	// TODO: add a way to consume more than zero arg. Neet to specify arrity.
}

var (
	_lock   sync.Mutex
	actions map[string]Action = make(map[string]Action)
)

type unlocker func()

func (u unlocker) unlock() { u() }
func lock() unlocker {
	_lock.Lock()
	return func() { _lock.Unlock() }
}

func Register(acts []Action) {
	defer lock().unlock()
	for _, act := range acts {
		actions[act.Name] = act
	}
}

func AsAction(name string, subactions []Action) Action {
	return Action{}
}

func Dispatch(args []string) func() { // TODO: add debug option to print dispatch chain
	defer lock().unlock()
	exit_if(len(args) == 0, "No argument")
	c := args[0]
	act, ok := actions[c]
	exit_if(!ok, fmt.Sprintf("Unknown command %s", c))
	return act.Fn
}

func exit_if(cond bool, msg string) {
	if !cond {
		return
	}
	fmt.Fprintln(os.Stderr, msg)
	os.Exit(1)
}
