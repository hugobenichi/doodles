def mult(n: Long)(m: Int): String = (n * m).toString

def concatMult(n: Long, m: Int): String = 1 to m map mult(n) mkString ""

def isPandigital(s: String): Boolean =
  s.size == 9 && s.toSet.filter(_ != '0').size == 9

def pandigitals(n: Long): List[Long] = {
  def loop(n: Long, l: Int): List[Long] = {
    val ps = concatMult(n,l)
    if (ps.size > 12) Nil else
      if (isPandigital(ps)) ps.toLong :: loop(n, l+1) else loop(n, l+1)
  }
  loop(n,2)
}


(List range[Long] (1, 10000) flatMap pandigitals).sorted foreach println
