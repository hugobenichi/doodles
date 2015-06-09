def sign(n: Int): List[Char] = n.toString.toList.sorted

def isValid(n: Int): Boolean = {
  val s = sign(2*n)
  sign(3*n) == s && sign(4*n) == s && sign(5*n) == s && sign(6*n) == s
}

(1 to 1000000) filter isValid foreach println
