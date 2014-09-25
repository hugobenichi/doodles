//  -- Simple ------------------------------------------------------------------
//
//  Simple is a simple interpreter that currently lacks a parser and a syntax.
package main

import (
  "fmt"
  "errors"
)

//  -- Errors ------------------------------------------------------------------

/** returned when a lookup in the environement fails. */
func UnboundErr(name string) error {
  return errors.New(fmt.Sprintf("Unbound variable '%s'", name))
}

/** returned when a type-check fails. */
func TypeErr(found, expected string) error {
  return errors.New(fmt.Sprintf("Value of type '%s' instead of '%s'", found, expected))
}


//  -- Types -------------------------------------------------------------------

type TypeInfo string

const (
  TypeInt TypeInfo = "Int"
  TypeFun TypeInfo = "Int -> Int"
  TypeList TypeInfo = "List"
  TypeAtom TypeInfo = "Atom"
)

type Val interface {
  Type() TypeInfo
  String() string
}

func assertType(needed TypeInfo, values ...Val) {
  for _, v := range values {
    if found := v.Type(); found != needed {
      panic(TypeErr(string(needed), string(found)))
    }
  }
}


//  -- Env ---------------------------------------------------------------------
//
//  Env is a map of stacks used to track and remember (variable,value) mapping
//  in nested scopes. Getting a value from the env with a variable name will
//  return the last pushed value at with that variable name binding. Hence the
//  env is implemented as a map[string,stack], with stacks implemented directly
//  with builtin slices and append().

type Env map[string][]Val

func EmptyEnv() Env {
  return Env(make(map[string][]Val))
}

func (e Env) Push(name string, v Val) {
  if _, has := e[name]; !has {
    e[name] = make([]Val, 0, 10)
  }
  e[name] = append(e[name], v)
}

func (e Env) Pop(name string) {
  v, ok := e[name]
  if !ok { return }
  if len(v) == 0 {
    delete(e, name)
    return
  }
  e[name] = v[0:len(v)-1]
}

func (e Env) Get(name string) Val {
  v, ok := e[name]
  if !ok {
    panic(UnboundErr(name))
  }
  return v[len(v)-1]
}


// -- Concrete Values ----------------------------------------------------------

// -- Int values ---------------------------------------------------------------

type Int int

func (n Int) Type() TypeInfo { return TypeInt }
func (n Int) String() string { return fmt.Sprintf("%d", int(n)) }

// numerical litterals evaluates to themselves
func (n Int) Eval(e Env) Val {
  return n
}

// -- Atom values --------------------------------------------------------------

type Atom string

func (a Atom) Type() TypeInfo { return TypeAtom }
func (a Atom) String() string { return fmt.Sprintf("Atom(%s)", string(a)) }

// Atoms evaluate to the value they are pointing to in the given environment
func (a Atom) Eval(e Env) Val {
  return e.Get(string(a))
}

type FVal struct {
  Variables []string
  Body Exp
  Name string
}

func (f FVal) Type() TypeInfo { return TypeFun }
func (f FVal) String() string { return "fun" }

//  -- Exp ---------------------------------------------------------------------
//
//  An Expression is a closure typed Env -> Val which when invoked eval itself
//  against the given environment. TODO: add errors return types

type Exp interface {
  Eval(e Env) Val
}


// not a closure yet. For that, I would need to include an env by either:
//  1) copying the env when the function body is declared
//  2) extract the vars inside the closure and copy only this in a new env
//  3) extract and replace the vars inside the closure body
func (fv FVal) Eval(e Env) Val {
  return fv
}


//  -- List --------------------------------------------------------------------
//
// Lists is a Val

type List struct {
  Head Exp
  Tail *List
}

func (l List) Type() TypeInfo { return TypeList }

func (l List) String() string { return "a List" }

// the empty List   !!!! baka, this should be nil !!!!
var Nil = List{}

// list creation primitive, need to be in the environment
func Cons(e Exp, l List) List {
  return List{ e, &l }
}


// -- ListEnv ------------------------------------------------------------------
//
// A functional version of the environment, with structural sharing, and no
// internal mutation. This allows to implement closures.

// instead of storing a list of (name, values) tuples,
// we store a (list atom, list values) tuples, which makes things a bit easier.
type ListEnv struct {
  Atoms *List
  Values *List
}

func (le ListEnv) Get(a Atom) Exp {
  atoms := le.Atoms
  values := le.Values
  for atoms != nil && values != nil {
    if atoms.Head.(Atom) == a {
      return values.Head
    }
    atoms = atoms.Tail
    values = values.Tail
  }
  panic(UnboundErr(string(a)))
}

// facility type for returning Env -> Val functions as Exp typed values.
type ExpFunc func(e Env) Val

func (ef ExpFunc) Eval(e Env) Val {
  return ef(e)
}

type PlusExp struct { x, y Exp }

func Plus(x, y Exp) Exp {
  return ExpFunc(func(e Env) Val {
    xval := x.Eval(e)
    yval := y.Eval(e)
    assertType(TypeInt, xval, yval)
    return xval.(Int) + yval.(Int)
  })
}

func Mult(x, y Exp) Exp {
  return ExpFunc(func(e Env) Val {
    xval := x.Eval(e)
    yval := y.Eval(e)
    assertType(TypeInt, xval, yval)
    return xval.(Int) * yval.(Int)
  })
}

func Less(x, y Exp) Exp {
  return ExpFunc(func(e Env) Val {
    xval := x.Eval(e)
    yval := y.Eval(e)
    assertType(TypeInt, xval, yval)
    if xval.(Int) < yval.(Int) {
      return Int(1)
    }
    return Int(0)
  })
}

func If(cond, x, y Exp) Exp {
  return ExpFunc(func(e Env) Val {
    if cond.Eval(e) == Int(0) {
      return y.Eval(e)
    }
    return x.Eval(e)
  })
}

func LetIn(name string, x, y Exp) Exp {
  f := func(e Env) Val {
    e.Push(name, PrepareVal(name, x, e))
    u := y.Eval(e)
    e.Pop(name)
    return u
  }
  return ExpFunc(f)
}

// helper function for LetIn
func PrepareVal(name string, x Exp, e Env) Val {
    v := x.Eval(e)
    // this allows for recursion in function
    if fval, is_function := v.(FVal); is_function {
      fval.Name = name
      return fval // needs to return fval and not v (not working with pointers)
    }
    return v
}

func Func(body Exp, variables ...string) Exp {
  return FVal { Variables: variables, Body: body }
}

// types with Exp or Vals ? Exp right !
// change name of values expression
func Call(funexp Exp, values ...Exp) Exp {
  f := func(e Env) Val {
    funval := funexp.Eval(e)
    assertType(TypeFun, funval)
    fun := funval.(FVal)

    // create a new env for the call frame
    funenv := EmptyEnv()

    // bind the func to the new env if it has a name (recursion)
    if fun.Name != "" {
      funenv.Push(fun.Name, fun)
    }

    // bind variables names from the func expression to the passed values
    minLen := len(values)
    if len(fun.Variables) < minLen { minLen = len(fun.Variables) }
    for i := 0; i < minLen; i++ {
      funenv.Push(fun.Variables[i], values[i].Eval(e))
    }

    // function body execution
    x := fun.Body.Eval(funenv)

    // unbind variables from the env
    //for i := 0; i < minLen; i++ {
    //  funenv.Pop(fun.Variables[i])
    //}
    return x
  }
  return ExpFunc(f)
}

func main() {
  e := EmptyEnv()
  e.Push("foo", Int(7))
  r := Plus(Mult(Int(4), Atom("foo")), Int(7))
  s := LetIn("bar", r, Plus(Atom("foo"), Atom("bar")))
  fmt.Println(s.Eval(e))

  f := Func(Plus(Atom("x"), Atom("y")), "x", "y")
  a := Call(f, Int(1), Int(5))
  b := Call(f, Int(10), Mult(Int(2), Int(5)))
  fmt.Println(a.Eval(e), b.Eval(e))

  n := 5
  program := LetIn(
    "factorial",
    Func(
      If(
        Atom("x"),
        Mult(
          Atom("x"),
          Call(Atom("factorial"), Plus(Atom("x"), Int(-1))),
        ),
        Int(1),
      ),
      "x",
    ),
    Call(Atom("factorial"), Int(n)),
  )
  fmt.Println(program.Eval(EmptyEnv()))
}
