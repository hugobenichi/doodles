object Bool {

  object Litteral {
    def apply(sym: Char): Exp = sym match {
      case '1' => True
      case '0' => False
      case _ => throw new IllegalArgumentException("not a litteral: " + sym)
    }
  }

  trait Exp {
    def value: Boolean
  }

  case object True extends Exp {
    def value = true
    override def toString = "1"
  }

  case object False extends Exp {
    def value = false
    override def toString = "0"
  }

  case class Node(op: Op, left: Exp, right: Exp) extends Exp {
    def value = op(left.value, right.value)
    override def toString = "(" + left + op + right + ")"
  }

  object Op {
    def apply(sym: Char): Op = sym match {
      case '&' => And
      case '|' => Or
      case '^' => Xor
      case _ => throw new IllegalArgumentException("not an operator: " + sym)
    }
  }

  trait Op {
    def apply(x: Boolean, y: Boolean): Boolean
    def solve(z: Boolean, x: Boolean): List[Boolean]
  }

  case object And extends Op {
    def apply(x: Boolean, y: Boolean) = x & y
    def solve(z: Boolean, x: Boolean) = (z,x) match {
      case (true, false) => Nil
      case (true, true) => true :: Nil
      case (false, false) => true :: false :: Nil
      case (false, true) => false :: Nil
    }
    override def toString = "&"
  }

  case object Or extends Op {
    def apply(x: Boolean, y: Boolean) = x | y
    def solve(z: Boolean, x: Boolean) = (z,x) match {
      case (true, false) => true :: Nil
      case (true, true) => true :: false :: Nil
      case (false, false) => false :: Nil
      case (false, true) => Nil
    }
    override def toString = "|"
  }

  case object Xor extends Op {
    def apply(x: Boolean, y: Boolean) = x ^ y
    def solve(z: Boolean, x: Boolean) = (z,x) match {
      case (true, false) => true :: Nil
      case (true, true) => false :: Nil
      case (false, false) => false :: Nil
      case (false, true) => true :: Nil
    }
    override def toString = "^"
  }

  def main(args: Array[String]) {
    rec(Litteral(args(0)(0)).value, args(1).toList) foreach println
  }

  def rec(rez: Boolean, exp: List[Char]): List[Exp] = exp match {
    case Nil => Nil
    case c :: Nil =>
      val lit = Litteral(c)
      if (lit.value == rez) lit :: Nil else Nil
    case litteral :: symbol :: tail =>
      val op = Op(symbol)
      val left = Litteral(litteral)
      op.solve(rez, left.value)
        .flatMap { rec(_, tail) }
        .map { Node(op, left, _) }
  }

  def rec2(rez: Boolean, exp: List[Char]): List[Exp] = exp match {
    case Nil => Nil
    case c :: Nil =>
      val lit = Litteral(c)
      if (lit.value == rez) lit :: Nil else Nil
    case lit :: sym :: tail =>

  }

}
