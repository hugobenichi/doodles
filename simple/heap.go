//  -- GLisp heap --------------------------------------------------------------
//
//  A heap/stack memory interface for GLisp
//
//
package main

import "fmt"

//  -- Basic Values ------------------------------------------------------------

// Represent a value in the heap. All values are encoded as 8B unsigned integers
// with the following scheme:
//  ints:   | 2b tag = 01 | 30b '0' padding | 32b int32 value |
//  atoms:  | 2b tag = 10 | 30b '0' padding | 32b int32 index in string table |
//  cells:  | 2b tag = 11 | 31b int32 ref | 31b int32 ref |
//  nil:    | 2b tag = 00 | 0 |
//
// in the case of cells the non-tag 62 bits are interpreter as two concatenated
// 31bits positive ints (equivalent to half of a signed int32). Both ints are
// Ref index values into the heap.
type Val struct {
	Type  int32
	Value uint64
}

type Ref int32

const nilVal = 0
const intVal = 1
const atmVal = 2
const celVal = 3

func Nil() Val { return Val{Type: nilVal, Value: 0} }

func isRef(v Val) bool  { return v.Type == celVal }
func HeadRef(v Val) Ref { return Ref(int32(v.Value >> 32)) }
func TailRef(v Val) Ref { return Ref(int32(v.Value)) }

// explain how strings are encoded for now
type Heap struct {
	Mem           []Val
	Symbols       []string
	SymbolMapping map[string]Ref
}

func NewHeap() *Heap {
	return &Heap{SymbolMapping: make(map[string]Ref)}
}

func (h *Heap) PushAtom(s string) Ref {
	if r, ok := h.SymbolMapping[s]; ok {
		return r
	}
	idx := uint64(len(h.Symbols))
	r := h.PushVal(Val{Type: atmVal, Value: idx})
	h.Symbols = append(h.Symbols, s)
	h.SymbolMapping[s] = r
	return r
}

func (h *Heap) PushNum(i int64) Ref {
	return h.PushVal(Val{Type: intVal, Value: uint64(i)})
}

func (h *Heap) PushNil() Ref {
	return h.PushVal(Nil())
}

func (h *Heap) PushCell(head, tail Ref) Ref {
	v := (uint64(int32(head)) << 32) + uint64(int32(tail))
	return h.PushVal(Val{Type: celVal, Value: v})
}

func (h *Heap) PushVal(v Val) Ref {
	h.Mem = append(h.Mem, v)
	return Ref(len(h.Mem) - 1)
}

func (h *Heap) Compact(root Ref, limit Ref) Ref {
	if !isRef(root) {
		// push root just after limit
		// truncate heap Mem
		// return moved Ref
	}
	// otherwise do compaction
	return root
}

func main() {
	fmt.Println("tos")
}

// small string strategies
//    always cache small strings in a hash ?
