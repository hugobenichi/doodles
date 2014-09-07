package main

import "fmt"

// dummy type for tokens for now
type Token string

// A parser is a function that consomes some bytes from a string.
// If the parse can consume some bytes, it returns a list of tokens, and a
// substring of the input string that was not consumed to produce the tokens.
// If it fails, it return an error.
type Parser func(string) ([]Token, string, error)

// can't remember how to create en error idiomatically
type Err string
func (e Err) Error() string {
  return string(e)
}

func Or(p, q Parser) Parser {
  fct := func(s string) ([]Token, string, error) {
    toks, rest, err := p(s)
    if err != nil {
      return q(s)
    }
    return toks, rest, err
  }
  return Parser(fct)
}

func And(p, q Parser) Parser {
  fct := func(s string) ([]Token, string, error) {
    toks1, t, err := p(s)
    if err != nil {
      return nil, s, err
    }
    toks2, rest, err := q(t)
    return append(toks1, toks2...), rest, err
  }
  return Parser(fct)
}

func Repeat(p Parser) Parser {
  fct := func(s string) ([]Token, string, error) {
    var all []Token
    for {
      toks, t, err := p(s)
      if err != nil {
        break
      }
      all = append(all, toks...)
      s = t
    }
    return all, s, nil
  }
  return Parser(fct)
}

func Seq(p, sep Parser) Parser {
  return And(p, Repeat(And(Skip(sep), p)))
}

func Const(pattern string) Parser {
  fct := func(s string) ([]Token, string, error) {
    if s[0:len(pattern)] == pattern {
      return []Token{Token(pattern)}, s[len(pattern):], nil
    }
    return nil, s, Err("could not parse '" + s + "'")
  }
  return Parser(fct)
}

func Skip(p Parser) Parser {
  fct := func(s string) ([]Token, string, error) {
    _, t, err := p(s)
    return nil, t, err
  }
  return Parser(fct)
}

func Digit() Parser {
  fct := func(s string) ([]Token, string, error) {
    c := s[0]
    if '0' <= c && c <= '9' {
      return []Token{Token(fmt.Sprintf("%x", c - '0'))}, s[1:], nil
    }
    return nil, s, Err("not a digit")
  }
  return Parser(fct)
}

func main() {

  fmt.Println("hello world")

  s := ":::abc"
  toks, t, err := Repeat(Const(":"))(s)
  fmt.Println(toks, t, err)

  s = "a,a,a,a,a,adfh"
  toks, t, err = Seq(Const("a"), Const(","))(s)
  fmt.Println(toks, t, err)

  s = "324f"
  d := Digit()
  toks, t, err = Repeat(d)(s)
  fmt.Println(toks, t, err)

}
