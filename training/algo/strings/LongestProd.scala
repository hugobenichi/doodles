import scala.math.Ordering
import scala.collection.SortedSet

object LongestProd {

  def prod(ls: Seq[String]): Int = {
    val ord = Ordering.by[String,Int]{ _.length }
    recurStrings(ls.foldLeft(SortedSet.empty(ord)) { _ + _ }.toStream, 0)
  }

  def recurStrings(seq: Stream[String], maxProd: Int): Int = seq match {
    case Stream.Empty => maxProd
    case s #:: tail if s.length * s.length <= maxProd => maxProd
    case s #:: tail =>
      val newMaxProd = tail.takeWhile { _.length * s.length > maxProd }
                           .filter { noLettersIn(_, s.toSet) }
                           .headOption
                           .map { case t => t.length * s.length }
                           .getOrElse(maxProd)
      recurStrings(tail, newMaxProd)
  }

  def noLettersIn(s: String, chars: Set[Char]) = !s.toSeq.exists(chars.contains)


}
