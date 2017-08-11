package main

/*

	Basic exercise to write a S-exp lexer and parser.

	The lexer is a made with a look-up table that map bytes to token types. The
	table can be used to detect forbidden bytes.

	The token types are fed to an automata implemented as a "two flat loops" with
	goto statements. In C the token types could be instead directly be jump
	pointers to save a switch.

	The AST is defined as a flattened sum type of Node or Atom, joined with a tail
	Node, similarly to a linked list.

	The Parse method builds the AST from the token stream by using a stack to
	push and pop nodes accordingly when going down or up the tree.

	Possible improvements:
	+ wrap the lexer with an iterative token stream that gets fed lines one by one
		and returns token one by one
			add a method that takes a token stream, a text diff, and generates the
			diffed token stream "smartly"
	+ think about error reporting from parse
		in particular add a link back to text from token stream and AST
*/

import "fmt"

/* ---- Lexer --------------------------------------------------------------- */

type TokenType uint8

const (
	Atom TokenType = iota
	Space
	LeftParen
	RightParen

	Forbidden = 255
)

var charTable = make([]TokenType, 256)

func initTable() {
	charTable[' '] = Space
	charTable['('] = LeftParen
	charTable[')'] = RightParen
}

type Token struct {
	t        TokenType
	from, to int
}

func (t Token) String(s string) string {
	switch t.t {
	case Atom:
		return s[t.from:t.to]
	case LeftParen:
		return "("
	case RightParen:
		return ")"
	default:
		return "?"
	}
}

func Lex(s string) []Token {
	var (
		cursor       = 0
		stash        = 0
		current_type TokenType
		tokens       []Token
	)

not_atom_loop:
	if cursor == len(s) {
		goto exit
	}
	current_type = charTable[s[cursor]]
not_atom:
	switch current_type {
	case Atom:
		stash = cursor
		goto atom
	case Space /* noop, skip to next byte */ :
	default /* special char */ :
		tokens = append(tokens, Token{t: current_type, from: cursor})
	}
	cursor++
	goto not_atom_loop

atom_loop:
	if cursor == len(s) {
		tokens = endWorld(tokens, stash, cursor)
		goto exit
	}
	current_type = charTable[s[cursor]]
	if current_type != Atom {
		tokens = endWorld(tokens, stash, cursor)
		goto not_atom
	}
atom:
	cursor++
	goto atom_loop

exit:
	return tokens
}

func LexOriginal(s string) []Token {
	var (
		cursor       = 0
		stash        = 0
		current_type TokenType
		tokens       []Token
	)

check_end:
	if cursor == len(s) {
		goto exit
	}

not_atom:
	current_type = charTable[s[cursor]]
	switch current_type {
	case Space:
		// noop, skip to next byte
	case Atom:
		goto atom
	default /* special char */ :
		tokens = append(tokens, Token{t: current_type})
	}
	cursor++
	goto check_end

atom:
	stash = cursor
	for current_type == Atom {
		cursor++
		if cursor == len(s) {
			return endWorld(tokens, stash, cursor)
		}
		current_type = charTable[s[cursor]]
	}
	tokens = endWorld(tokens, stash, cursor)
	goto not_atom

exit:
	return tokens
}

func endWorld(tokens []Token, from, to int) []Token {
	return append(tokens, Token{t: Atom, from: from, to: to})
}

/* ---- Abstract Syntax Tree ------------------------------------------------ */

type Node struct { // Node should be Atom x Tail or Head x Tail.
	Atom string
	Head *Node
	Tail *Node
}

func (n *Node) Print() {
	n.print_indent("")
}

func (n *Node) print_indent(prefix string) {
	for n != nil {
		switch {
		case n.Atom != "":
			fmt.Println(prefix + n.Atom)
		case n.Head != nil:
			fmt.Println(prefix + "(")
			n.Head.print_indent(prefix + "  ")
			fmt.Println(prefix + ")")
		}
		n = n.Tail
	}
}

func Parse(s string, ts []Token) (*Node, error) {
	var (
		stack  []*Node
		cursor = -1
		top    = &Node{}
		root   = top

		next **Node
	)
	for _, t := range ts {
		switch t.t {
		case Atom:
			top.Atom = t.String(s) // not ideal: should wrap string ?
			next = &top.Tail
		case LeftParen:
			// push node
			cursor++
			stack = append(stack, nil)
			stack[cursor] = top
			next = &top.Head
		case RightParen:
			// pop node
			if cursor < 0 {
				return nil, fmt.Errorf("did not expect right paren at %d", t.from)
			}
			top = stack[cursor]
			cursor--
			next = &top.Tail

			// add case for error tokens
		}
		top = &Node{}
		*next = top
	}

	if cursor >= 0 {
		// TODO: pass down left paren position, print all missing paren position
		return nil, fmt.Errorf("left paren not closed")
	}
	return root, nil
}

func CanParse(ts []Token) bool {
	paren := 0
	for _, t := range ts {
		switch {
		case t.t == LeftParen:
			paren++
		case t.t == RightParen && paren == 0:
			return false
		case t.t == RightParen:
			paren--
		}
	}
	return paren == 0
}

func main() {
	initTable()
	s := "  hello (world I am) a (visitor from    space) (and a tos)"

	//for _, t := range Lex(s) {
	//	fmt.Println(t.String(s))
	//}

	//fmt.Println(CanParse(Lex(s)))

	ast, err := Parse(s, Lex(s))
	fmt.Println(err)
	ast.Print()

	n := Node{
		Atom: "hello",
		Tail: &Node{
			Head: &Node{
				Atom: "world",
				Tail: &Node{
					Atom: "I",
					Tail: &Node{
						Atom: "am",
					},
				},
			},
			Tail: &Node{
				Atom: "a",
				Tail: &Node{
					Head: &Node{
						Atom: "visitor",
						Tail: &Node{
							Atom: "from",
							Tail: &Node{
								Atom: "space",
							},
						},
					},
				}},
		}}

	if false {
		n.Print()
	}
}
