package red

//import "col"

// ---- interface declaration --------------------------------------------------

// ---- Reducer bits

type Object interface{}

type Combiner func(Object, Object) Object

// this is essentially an enumerate trait
// should I declare it with the same reduce type of: x -> (x -> y -> x) -> x ??
type Red interface {
  Traverse(func(Object))
  Reduce(seed Object, c Combiner) Object
}

// ---- constructing interfaces

type Col interface {
  IsEmpty() bool
  Add(Object) Col
  Empty() Col
  Union(Col) Col
  Seq() Seq
}


// ---- user interfaces

// immutable Sequence
type Seq interface {
  Col
  Head() (Object, bool)
  Tail() (Seq, bool)
}

// mutable outer iterator
type Iter interface {
  Get() Object
  Next() bool
}

// ---- make an iterator from a Seq

type SeqIterator struct {
  Init bool
  S Seq
}


func (si *SeqIterator) Get() Object {
  o, _ := si.S.Head()
  return o
}

func (si *SeqIterator) Next() bool {
  if si.Init && !si.S.IsEmpty() {
    si.S, _ = si.S.Tail()
  } else {
    if si.Init {
      return false
    } else {
      si.Init = true
    }
  }
  return true
}

func IterOf(s Seq) Iter {
  return &SeqIterator{ Init: false, S: s }
}

// does not work !!
func ToArray(i Iter) {
  ary := make([]interface{}, 10)[0:0]
  for i.Next() {
    ary = append(ary, i.Get())
  }
}

// ---- Seq implementation -----------------------------------------------------

// linked list
type List struct {
  Value Object
  Next Seq
}

type ListLL struct {


func (l List) Head() (Object, bool) { return l.Value, true }

func (l List) Tail() (Seq, bool) { return l.Next, true }

func (l List) Seq() Seq { return l }

func (l List) IsEmpty() bool { return false }

func (l List) Add(o Object) Col { return List{Value: o, Next: l} }

func (l List) Empty() Col { return Empty }

func (l List) Union(c Col) Col { return l }


type EmptyList struct {}

var Empty EmptyList = EmptyList{}

func (e EmptyList) Head() (Object, bool) { return nil, false }

func (e EmptyList) Tail() (Seq, bool) { return nil, false }

func (e EmptyList) Seq() Seq { return e }

func (e EmptyList) IsEmpty() bool { return true }

func (e EmptyList) Add(o Object) Col { return List{Value: o, Next: e } }

func (e EmptyList) Empty() Col { return Empty }

func (e EmptyList) Union(c Col) Col { return c }



// ---- Reducer implementation -------------------------------------------------


type FRed func(func(Object))

func (fr FRed) Traverse(f func(Object)) {
  fr(f)
}

func (fr FRed) Reduce(seed Object, c Combiner) Object {
  accum := seed
  reducer := func (x Object) {
    accum = c(accum, x)
  }
  fr(reducer)
  return accum
}

func Into(r Red, c Col) Col {
  output := c
  reducer := func (x Object) {
      output = c.Add(x)
  }
  r.Traverse(reducer)
  return output
}

func Map(r Red, f func (Object) Object) Red {
  s := func(g func(Object)) {
    r.Traverse(func(x Object) { g(f(x)) })
  }
  return FRed(s)
}


// ---- just a test ------------------------------------------------------------

func doSometing() {
  fr := func(f func(Object)) {
      x := "foobar"
      y := 43
      z := make([]int, 10)
      f(x)
      f(y)
      f(z)
  }
  r := FRed(fr)
  t := Map(r, func(x Object) Object { return x })
  if t != nil {
      r = FRed(fr)
  }
}

/*
a bit more verbose, but a bit nicer to understand
func Map2(r Red, f func (Object) Object) Red {
  wrapper := func(seed Object, c Combiner) Object {
      adapter := func(accum Object, x Object) Object {
          return c(accum, f(x))
      }
      return r.Reduce(seed, adapter)
  }
  return FRed(wrapper)
}
*/
