import scala.annotation.tailrec

def primesRec(p: Array[Int]): Unit = {
  def mark(n: Int, p: Array[Int]): Int = {
    var i = n + n
    while (i > 0 && i < p.length) {
      p(i) = -1
      i += n
    }
    var j = n+1
    while (j > 0 && j < p.length && p(j) < 0) {
      j += 1
    }
    return j
  }
  @tailrec
  def loop(n: Int, p: Array[Int]): Unit = if (n < p.length) loop(mark(n,p), p)
  loop(2, p)
}

val ps = (0 to 1000000000).toArray
primesRec(ps)

case class Count(n: Int, rank: Int, diag: Int, primes: Int) {
  def next(): Count = {
    val a = n + rank
    val b = a + rank
    val c = b + rank
    val d = c + rank

    var p = primes
    if (ps(a) > 0) p += 1
    if (ps(b) > 0) p += 1
    if (ps(c) > 0) p += 1
    if (ps(d) > 0) p += 1

    return Count(d, rank + 2, diag + 4, p)
  }

  def ratio = primes * 1.0 / diag
}

val a = Count(1,2,1,0).next

def squares(c: Count = a): Stream[Count] = c #:: squares(c.next)

def stop(c: Count) = c.n + 4*c.rank < ps.length && c.ratio >= 0.10

squares() takeWhile stop foreach println // don't forget to add one to rank
