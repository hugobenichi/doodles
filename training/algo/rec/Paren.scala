import scala.annotation.tailrec

object Paren {

  def balance(exp: List[Char]): Boolean = count(0, exp)

  @tailrec
  def count(left: Int, exp: List[Char]): Boolean = (left, exp) match {
    case (0, Nil) => true
    case (_, Nil) => false
    case (0, ')' :: _ ) => false
    case (_, ')' :: tail) => count(left-1, tail)
    case (_, '(' :: tail) => count(left+1, tail)
    case (_, _ :: tail) => count(left, tail)
  }

  def main(args: Array[String]) {
    Console println balance(args(0).toList)
  }

}
