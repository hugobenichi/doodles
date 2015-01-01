import scala.annotation.tailrec
import scala.collection.mutable.ListBuffer
import scala.util.{Try, Success, Failure}

object Lexer {

  trait State
  case object Done extends State
  case object NeedMore extends State

  type Result[Token] = (State, Seq[Token])

}

trait Lexer[Token] {

  import Lexer.Result

  def scan(line: String): Result[Token]

}

object LexerUser {

  import Lexer._

  def scanWith[T](lines: Seq[String], l: Lexer[T]): Try[Seq[T]] = {

    @tailrec
    def rec(line: Seq[String], tokens: Stream[T], state: State): Result[T] =
        line match {
            case Nil =>
                (state, tokens)
            case line :: tail =>
              val (newState, newTokens) = l scan line
              rec(tail, tokens ++ newTokens, newState)
        }

    rec(lines, Stream.empty[T], NeedMore) match {
        case (NeedMore, _) =>
            Failure(new IllegalArgumentException("incomplete data"))
        case (Done, tokens) =>
            Success(tokens)
    }
  }

}

object SExpression {

  trait SToken
  trait SExp

  case object LeftPar extends SToken
  case object RightPar extends SToken
  case class Atom(w: String) extends SToken with SExp

  case class SList(subexps: Seq[SExp]) extends SExp

  object Space {
    def unapply(c: Char): Boolean = (c == ' ' || c == '\t')
  }

  object Atom {
    def findEnd(s: String, i: Int): Int = {
      if (i >= s.length) return i
      val c = s(i)
      if (c == '(' || c ==')' || c == ' ' || c == '\t') i
      else findEnd(s, i+1)
    }
  }

  def toTokens(s: SExp): Seq[SToken] = s match {
      case a: Atom =>
          Seq(a)
      case SList(subexps) =>
          Seq(LeftPar) ++ (subexps flatMap toTokens) ++ Seq(RightPar)
    }

  def toSExp(ts: Seq[SToken]): SExp = SList(Nil)
}

object SGen {

  import SExpression._
  import scala.util.Random

  def apply(): SExp = genSList

  def gen: SExp = if (Random.nextInt(100) > 70) genSList else genAtom

  def genSList = SList(List.fill(Random nextInt 5) { SGen.gen })

  def genAtom = Atom(Random.alphanumeric.take(1 + Random.nextInt(10)).mkString)
}

val s = SGen()

Console println s
Console println SExpression.toTokens(s)

// add comments parser
class SLexer extends Lexer[SExpression.SToken] {

  import SExpression._
  import Lexer._

// parser combinator
// many_n (apply n or more time a parser)
// any (apply any of given list of parsers)
// both (apply two parsers in succession)
// all (apply all parsesr in succession)
// sepb (apply parser a n times with b repeated)

  def scan(line: String) = {
    val toks = ListBuffer[SToken]()
    @tailrec
    def rec(idx: Int): Unit = {
      if (idx >= line.length) return
      val offset = line(idx) match {
        case '(' =>
          toks += LeftPar
          1
        case ')' =>
          toks += RightPar
          1
        case Space() =>
          1
        case _ =>
          val nextIdx = Atom findEnd(line, idx)
          toks += new Atom(line substring (idx, nextIdx))
          nextIdx
      }
      rec(idx + offset)
    }
    (Done, Nil)
  }
}

/*

#t and #f evaluates to true and false
quote
  will takes one argument which is returned directly (without being evaluated).
atom
  also takes a single argument, and returns true or false depending on whether the argument is an atom.
eq
  returns true if both its arguments are the same atom, and false otherwise.
The arithmetic operators (+, -, *, /, mod and >) all take two arguments, and does exactly what you would expect.

*/
