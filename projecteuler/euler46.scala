import scala.collection.mutable.ListBuffer

def primesRec(ls: List[Int], buf: ListBuffer[Int] = ListBuffer[Int]()): List[Int] = ls match {
  case Nil => buf.toList
  case h :: t =>
    buf += h
    primesRec(t filter { n => (n % h) != 0 }, buf)
}

val ls = primesRec((2 to 10000).toList)

val ps = 1 to 2000 map { x => 2 * x * x }

def add(ns: Set[Int], x: Int): Set[Int] = ns + x

def foldOnPrime(ns: Set[Int], p: Int): Set[Int] =
  (ps.foldLeft(ns) { case (s,q) => add(s,p+q) }) + p

val all = ls.foldLeft(Set.empty[Int])(foldOnPrime)

// brute force approach by upsizing ls and ps until we find a good candidate.
(1 to 10000) map { 1 + 2 * _ } filter { !all(_) } foreach println
