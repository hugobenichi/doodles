//  -- GLisp -------------------------------------------------------------------
//
//  A basic lisp interpreter in go
//
//  left to implement
//    - define intrinsic
//    - do some basic testing
//    - turn ListEnv into regular List for closures
//    - implement closures in Fn
//    - try the Ycomb
//    - add defn to define recursive functions
//
package main

import (
  "fmt"
  "errors"
)

// tag interface to define the ensemble of possible values
type Val interface {
  implement()
  String() string
}

//  -- Basic Values ------------------------------------------------------------
//  are also expressions.

type Atom string

type Num int

type List struct {
  Head Val
  Tail *List
}

type Intrinsic func(*List) Val

func (a Atom) implement() { }
func (a Atom) String() string { return fmt.Sprintf("Atom(%s)", string(a)) }

func (n Num) implement() { }
func (n Num) String() string { return fmt.Sprintf("%d", int(n)) }

func (l *List) implement() { }
func (l *List) String() string { return fmt.Sprintf("%#v", l) }

func (l *List) Cons(v Val) *List {
  return &List{ v, l }
}

func (i Intrinsic) implement() { }
func (i Intrinsic) String() string { return "Intrinsic" }


//  -- Environment

type Env interface {
  Get(a Atom) Val
  Push(a Atom, v Val) Env
}


//  -- ListEnvironment

// TODO: I need to change this into a regular List

type ListEnv struct {
  Atoms *List
  Values *List
}

func (le ListEnv) Get(a Atom) Val {
  atoms := le.Atoms
  values := le.Values
  for atoms != nil && values != nil {
    if atoms.Head.(Atom) == a {
      return values.Head
    }
    atoms = atoms.Tail
    values = values.Tail
  }
  panic(errors.New(fmt.Sprintf("Unbound variable '%s'", string(a))))
}

func (le ListEnv) Push(a Atom, v Val) Env {
  return ListEnv{ le.Atoms.Cons(a), le.Values.Cons(v) }
}

func EmptyEnv() Env {
  return ListEnv { }
}

// -- Evaluator

func Eval(e Env, v Val) Val {
  switch t := v.(type) {

  default:
    panic(errors.New(fmt.Sprintf("Cannot evaluate value %s", v)))

  // A literal number simply evaluates to itself.
  case Num:
    return t

  // An Atom evaluates to its bound values in the environement.
  case Atom:
    return e.Get(t)

  // A List node evaluates to a a function call
  case *List:
    if t == nil {
      panic("Cannot call the empty list")
    }

    fn := Eval(e, t.Head)
    vals := t.Tail

    // if fn is an Intrasic, call it
    if intrinsic, ok := fn.(Intrinsic); ok {
      return intrinsic(t.Tail)
    }

    // otherwise the head should eval to a function shape
    fnshape := fn.(*List)
    // needs to turn ListEnv into a Val if I want to represent functions as vals
    _ = fnshape.Head //.(ListEnv)
    cloz := EmptyEnv() // for the time being, no closures
    body := fnshape.Tail.Head
    vars := fnshape.Tail.Tail

    // eval the tail expressions and bind them to the names in var
    for vars != nil {
      cloz = cloz.Push(vars.Head.(Atom), Eval(e, vals.Head))
      vars = vars.Tail
      vals = vals.Tail
    }

    return Eval(cloz, body) // now, simply eval body with the cloz "call frame"
  }
}


// -- Intrinsic functions

var Fn = Intrinsic(func(l *List) Val { return l.Head })

var Head = Intrinsic(func(l *List) Val { return l.Head })

var Tail = Intrinsic(func(l *List) Val { return l.Tail })

// exactly the same ??
var Cons = Intrinsic(func(l *List) Val { return l.Tail.Cons(l.Head) })

var Ls = Intrinsic(func(l *List) Val { return l })

var LetIn = 0
var If = 0
var Plus = 0
var Mult = 0


// head, tail, cons
// fn
// if then else
// let, letrec

// -- Main program

func main() {
  fmt.Println("")
}
