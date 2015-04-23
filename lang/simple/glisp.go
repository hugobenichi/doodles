//  -- GLisp -------------------------------------------------------------------
//
//  A basic lisp interpreter in go
//
//  left to implement
//    - try the Ycomb
//    - add defn to define recursive functions
//
package main

import (
  "fmt"
  "errors"
)


//  -- Basic Values ------------------------------------------------------------


// tag interface to define the ensemble of possible values
type Val interface { String() string }

type Atom string
func (a Atom) String() string { return fmt.Sprintf("Atom(%s)", string(a)) }

type Num int
func (n Num) String() string { return fmt.Sprintf("%d", int(n)) }

type List struct {
  Head Val
  Tail *List
}
func (l *List) String() string { return fmt.Sprintf("%#v", l) }
func (l *List) Cons(v Val) *List { return &List{ v, l } }

type Intrinsic func(environment Val, args *List) Val
func (i Intrinsic) String() string { return "Intrinsic" }


//  -- Environment getter and setter -------------------------------------------

func Get(env Val, a Atom) Val {
  ls := env.(*List)
  atoms := ls.Head.(*List)
  values := ls.Tail.Head.(*List)
  for atoms != nil && values != nil {
    if atoms.Head.(Atom) == a {
      return values.Head
    }
    atoms = atoms.Tail
    values = values.Tail
  }
  panic(errors.New(fmt.Sprintf("Unbound variable '%s'", string(a))))
}

func Push(env Val, a Atom, v Val) Val {
  ls := env.(*List)
  atoms := ls.Head.(*List)
  values := ls.Tail.Head.(*List)
  return list(atoms.Cons(a), values.Cons(v))
}


// -- Evaluator ----------------------------------------------------------------

func Eval(e Val, v Val) Val {
  switch t := v.(type) {

  default:
    panic(errors.New(fmt.Sprintf("Cannot evaluate value %s", v)))

  // A literal number simply evaluates to itself.
  case Num:
    return t

  // An Atom evaluates to its bound values in the environement.
  case Atom:
    return Get(e, t)

  // A List node evaluates to a a function call
  case *List:
    if t == nil {
      panic("Cannot call the empty list")
    }

    fn := Eval(e, t.Head)
    vals := t.Tail

    // if fn is an Intrasic, call it
    if intrinsic, ok := fn.(Intrinsic); ok {
      return intrinsic(e, t.Tail)
    }

    // otherwise the head should eval to a function shape
    fnshape := fn.(*List)
    cloz := fnshape.Head
    vars := fnshape.Tail.Head.(*List)
    body := fnshape.Tail.Tail.Head

    // eval the tail expressions and bind them to the names in var
    for vars != nil {
      cloz = Push(cloz, vars.Head.(Atom), Eval(e, vals.Head))
      vars = vars.Tail
      vals = vals.Tail
    }

    return Eval(cloz, body) // now, simply eval body with the cloz "call frame"
  }
}


// -- Intrinsic functions ------------------------------------------------------

// lists

var Head = Intrinsic(func(e Val, l *List) Val { return l.Head })
var Tail = Intrinsic(func(e Val, l *List) Val { return l.Tail })
var Ls = Intrinsic(func(e Val, l *List) Val { return l })

// exactly the same ??
var Cons = Intrinsic(func(e Val, l *List) Val { return l.Tail.Cons(l.Head) })

// control

// BUG: empty list does not trigger false branch
func If(e Val, l *List) Val {
  conditional := Eval(e, l.Head)
  if conditional == Num(0) || conditional == nil {
    return Eval(e, l.Tail.Tail.Head)
  }
  return Eval(e, l.Tail.Head)
}

func Let(e Val, l *List) Val {
  binding := l.Head.(*List)
  body := l.Tail.Head
  e = Push(e, binding.Head.(Atom), Eval(e, binding.Tail.Head))
  return Eval(e, body)
}

func Fn(e Val, l *List) Val {
  return l.Cons(e) // simply bind the current env to create the closure
}

// arithmetics

func Plus(e Val, l *List) Val {
  sum := 0
  for l != nil {
    v := Eval(e, l.Head)
    if x, ok := v.(Num); ok {
      sum += int(x)
    }
    l = l.Tail
  }
  return Num(sum)
}

func Mult(e Val, l *List) Val {
  prod := 1
  for l != nil {
    v := Eval(e, l.Head)
    if x, ok := v.(Num); ok {
      prod = prod * int(x)
    }
    l = l.Tail
  }
  return Num(prod)
}

var plus = Atom("+")
var mult = Atom("*")
var cond = Atom("if")
var let  = Atom("let")
var null = Atom("nil")
var fn   = Atom("fn")

var base_env Val = nil

func NewEnv() Val {
  if base_env == nil {
    base_env = list(list(), list())
    base_env = Push(base_env, plus, Intrinsic(Plus))
    base_env = Push(base_env, mult, Intrinsic(Mult))
    base_env = Push(base_env, cond, Intrinsic(If))
    base_env = Push(base_env, let,  Intrinsic(Let))
    base_env = Push(base_env, fn,   Intrinsic(Fn))
    base_env = Push(base_env, null, list())
  }
  return base_env
}


// -- Main program -------------------------------------------------------------


func list(vals ...Val) Val {
  var l *List = nil
  for i := len(vals) - 1; i >= 0; i-- {
    l = l.Cons(vals[i])
  }
  return l
}

func main() {
  x, y := Atom("x"), Atom("y")

  p1 := list(plus, Num(3), Num(10))
  fmt.Println(Eval(NewEnv(), p1))

  p2 := list(mult, Num(1), Num(2))
  fmt.Println(Eval(NewEnv(), p2))

  p3 := list(cond, null, Num(2), Num(3))
  fmt.Println(Eval(NewEnv(), p3))

  p4 := list(let, list(x, Num(2)), list(plus, x, Num(3)))
  fmt.Println(Eval(NewEnv(), p4))

  p5 := list(list(fn, list(x), list(mult, x, Num(8))), Num(3))
  fmt.Println(Eval(NewEnv(), p5))

  p6 := list(
          list(
            list(fn, list(x),
              list(fn, list(y),
                list(mult, x, y))),
            Num(5)),
          Num(4))
  fmt.Println(Eval(NewEnv(), p6))
}
