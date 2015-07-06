import scala.annotation.tailrec

def count(d: Int): Int = {
  @tailrec
  def loop(n: BigInt, t: Int): Int = {
    if (n > 9) return t                         // 10 ^ n have n+1 digits
    val p = n.pow(d)
    val l = p.toString.length
    if (l == d) println(d, n, p)
    loop(n+1, t + (if (l == d) 1 else 0))
  }
  loop(BigInt(1), 0)      // the solution excludes 0 from positive integers
}

Console println ((1 to 100) map count).sum
