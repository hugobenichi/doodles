bunch of expriments while reading www.buildyourownlisp.com

--------------------------------------------------------------------------------
what to do next:

  decide language syntax.
    -> for the moment, == AST, {'(', ')', [0-9]+}
  write parser to write AST
    -> decide on AST in-memory form
    -> check how much space it takes on the stack
        if keep source code in data structure as a *byte, that s not much
        I just need a nestable tree-like struct with a type
        how to deal with a variable-len list ?

  rewindable parser interface ?
    struct cursor(int line_num, line_offset)
    struct line{*byte, int len}
    init(max_memory, max_len_size)
    int read_lines()
    struct line line(parser, struct cursor)
    struct line next_line(parser, *cursor)
    struct line prev_line(parser, *cursor)
    boolean has_next(parser, struct cursor) ??

  how to parse ? try to batch lines ->
    one line -> list of tokens
      token is struct {
        cursor
        token type   // from enum
        token data   // union
      }

token = LeftParen
      | RightParen
      | Numeric { long value }
      | Atom { int nbytes }
      | Error { *char msg, int msg_len }

  parsing strategy
    alloc a token view for a filereader
    maintain a list of tokens
    loop for all lines
      push new tokens using parser

  parser -> (enforce ascii)
    offset cursor
    moves while space
      if end of line, adjust to next line ??
    if '(' return left paren; same for ')'
    if [0-9], continue to read numbers until next space, '(' or ')'
              number error if letters
    if [a-zA-Z:symbols], continue to read until next space, '(' or ')'

language elements
  numbers, +, -, *, &, |
  true, false, or , not
  >, <, <=, >=
  if then else
  def symbol, let
