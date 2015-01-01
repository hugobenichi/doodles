object LeadingZero {

  def countRec(n: Int, nBits: Int, nCount: Int): Int ={
    if (nBits == 0) return nCount + 1 - (n & 1)
    val mask = ((1 << nBits) - 1) << nBits
    Console println binRep(mask)
    if ((mask & n) != 0)
      return countRec(n >> nBits, nBits >> 1, nCount)
    else
      return countRec(n, nBits >> 1, nCount + nBits)
  }

  def main(argv: Array[String]) {
    val n = argv(0).toInt
    val leadZeros = countRec(n, 16, 0)
    Console println leadZeros
    Console println binRep(n)
    Console println binRep((1 << (32-leadZeros)) - 1)
  }

  def binRep(n: Int): String = {
    val tail = java.lang.Integer.toString(n, 2)
    "0" * (32-tail.length) + tail
  }

}
