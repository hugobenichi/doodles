package lock

/*
usage:
	var l lock.L
	func foo() {
		defer l.Lock().Unlock()
		 ...
	}
*/

import (
	"sync"
)

type L struct{ sync.Mutex }
type U func()

func (l *L) Lock() U {
	l.Mutex.Lock()
	return func() { l.Unlock() }
}
func (u U) Unlock() { u() }
