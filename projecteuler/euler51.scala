/*
for every prime p
  for every digit d
    generate all digit keys from the p
      where d has been changed to '*' and has not been changed
  regroup by keys
*/

import scala.collection.mutable.ListBuffer
import scala.annotation.tailrec

@tailrec
def primesRec(ls: List[Int], buf: ListBuffer[Int] = ListBuffer[Int]()): List[Int] =
  ls match {
    case Nil => buf.toList
    case h :: t =>
      buf += h
      primesRec(t filter { n => (n % h) != 0 }, buf)
  }

val ps = primesRec(List range (2,1000000))

def keys(d: String, ls: List[String]): List[List[String]] = {
  def loop(qs: List[String]): List[List[String]] = qs match {
    case Nil => List(Nil)
    case h :: t =>
      val ks = loop(t)
      val ps = ks map (h :: _)
      if (h == d) (ks map ("*" :: _)) ++ ps else ps
  }
  loop(ls) filter (_ contains "*")
}

def digits(n: Int): List[String] = n.toString.toList.map(_.toString)

val ds = digits(1234567890)

def allkeys(ls: List[String]): List[List[String]] = ds flatMap(keys(_, ls))

val tups = for {
  p <- ps
  ds = digits(p)
  ks <- allkeys(ds)
} yield (ks.mkString,p)

def regroup(t: (String, List[(String, Int)])) = (t._1, t._2.map(_._2))

tups groupBy (_._1) filter (_._2.size > 6) map regroup foreach println
