// invoque with $ scala -J-Xmx2g euler60.scala
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

val ps = (0 to 100000000).toArray
primesRec(ps)

val rng = 2 to 100000 filter (ps(_) > 0)

def isPrime(n: Long) = n < ps.length && ps(n.toInt) > 0

@tailrec
def powerTen(x: Long, n: Long = 10): Long = if (x < n) n else powerTen(x,10*n)

def join(a:Long, b:Long) = powerTen(b) * a + b

def isPair(a: Long, b: Long) = isPrime(join(a,b)) && isPrime(join(b,a))

val sols = for {
  a <- rng
  b <- rng
  if b > a && isPair(b,a)
  c <- rng
  if c > b && isPair(c,a) && isPair(c,b)
  d <- rng
  if d > c && isPair(d,a) && isPair(d,b) && isPair(d,c)
  e <- rng
  if e > d && isPair(e,a) && isPair(e,b) && isPair(e,c) && isPair(e,d)
} yield (a,b,c,d,e)

Console println sols.size

sols foreach println
