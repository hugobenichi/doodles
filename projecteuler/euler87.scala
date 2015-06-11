import scala.collection.mutable.ListBuffer

def primesRec(ls: List[Long], buf: ListBuffer[Long] = ListBuffer[Long]()): List[Long] =
  ls match {
    case Nil => buf.toList
    case h :: t =>
      buf += h
      primesRec(t filter { n => (n % h) != 0 }, buf)
  }

val ls = primesRec(List range[Long] (2,10000))

val lim = 50 * 1e6

val p2 = ls map { x => x * x} takeWhile( _ <= lim)
val p3 = ls map { x => x * x * x} takeWhile( _ <= lim)
val p4 = ls map { x => x * x * x * x} takeWhile( _ <= lim)

// numbers of primes multiples is small, so brute force approach works well.
// Console println p2.size => 914
// Console println p3.size => 73
// Console println p4.size => 23

val all = for {
  a <- p2
  b <- p3
  c <- p4
} yield a + b + c

Console println (all filter { _ < lim }).toSet.size
