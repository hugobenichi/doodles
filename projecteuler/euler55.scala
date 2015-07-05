def join(ls: List[String]): BigInt = BigInt(ls.mkString)

def explose(n: BigInt): List[String] = n.toString.toList.map(_.toString)

def revAdd(ls: List[String]): List[String] = explose(join(ls) + join(ls.reverse))

def isPal(ls: List[String]): Boolean = ls == ls.reverse

def isLyn(n: Int): Boolean = {
  def loop(ls: List[String], n: Int): Boolean = {
    if (n == 50) return true
    val qs = revAdd(ls)
    if (isPal(qs)) return false
    return loop(qs, n+1)
  }
  loop(explose(BigInt(n)), 0)
}

(1 to 9999) filter isLyn foreach println // pipe to wc -l
