object Queens {

  def makeRow(pos: Int): String = " " + "  " * pos + "X"

  def showBoard(positions: Array[Int]) {
    Console println "--" * (positions.length + 1)
    positions map makeRow foreach println
  }

  def toPosition(points: Iterable[(Int,Int)]): Array[Int] =
    points.foldLeft(new Array[Int](points.size)) {
        case (ary, (x,y)) => ary(x) = y; ary
    }

  def main(argv: Array[String]) {
    queens(argv(0).toInt, showBoard)
  }

  def queens(n: Int, consume: Array[Int] => Unit) {
    //rec((0 until n).toSet, Set.empty[(Int,Int)], consume)
    rec2((0 until n).toList, List.empty[(Int,Int)]) foreach {
        case lpos =>
            (consume compose toPosition _)(lpos)
    }
  }

  def rec(avail: Set[Int], taken: Set[(Int,Int)], consume: Array[Int] => Unit) {
    if (avail.isEmpty)
        consume(toPosition(taken))
    for {
        x <- avail
        if taken forall { case (a,b) => notDiag(x,taken.size,a,b) }
    } { rec(avail - x, taken + Tuple2(x,taken.size), consume) }
  }

  def notDiag(x: Int, y: Int, a: Int, b: Int) = (x - a).abs != (y - b).abs

  def comp(x: Int, y: Int, pos: List[(Int,Int)]) =
    pos forall { case (a,b) => notDiag(x,y,a,b) }

  def rec2(avail: List[Int], taken: List[(Int,Int)]): List[List[(Int,Int)]] =
    if (avail.isEmpty)
        taken :: Nil
    else
        avail filter { comp(_, taken.size, taken) } flatMap {
            case x =>
                rec2(avail filter { _ != x }, (x,taken.size) :: taken)
        }

  def rec3(avail: List[Int], taken: List[(Int,Int)]): Stream[List[(Int,Int)]] =
    avail match {
        case Nil =>
            Stream(taken)
        case x :: tail =>
            val others = rec3(tail, taken)
            if (comp(x,taken.size, taken))
                rec3(tail, (x,taken.size) :: taken) #::: others
            else others
    }

}
