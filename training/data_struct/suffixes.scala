
def suf[T](xs: List[T]): List[List[T]] = {
  def prependTo[T](x: T, xs: List[List[T]]) = xs match {
    case Nil => List(Nil)
    case y :: ys => (x :: y) :: y :: ys
  }
  xs match {
    case Nil => List(Nil)
    case x :: xs => prependTo(x, suf(xs))
  }
}

Console println suf(List(1,2,3,4))
