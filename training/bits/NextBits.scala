object NextBits {

  def next(n: Int): Int = {
    if (n == 0) return n
    val bit = firstZeroAfter(n, firstOneIn(n))
    val lomask = (1 << (bit-1))-1
    val himask = (~lomask) << 1
    return (n & himask ) | (1 << bit) | alignLeft(n & lomask)
  }

  def alignLeft(n: Int): Int =
    if (n == 0 || (n & 1) == 1) n else alignLeft(n >> 1)

  def firstOneIn(n: Int, bit: Int = 0): Int =
    if ((n & (1 << bit)) == 0) firstOneIn(n, bit + 1) else bit

  def firstZeroAfter(n: Int, bit: Int): Int =
    if ((n & (1 << bit)) == 0) bit else firstZeroAfter(n, bit + 1)

  def main(argv: Array[String]) {
    val n = java.lang.Integer.parseInt(argv(0),2)
    val m = next(n)
    Console println java.lang.Integer.toBinaryString(n)
    Console println java.lang.Integer.toBinaryString(m)
  }

}
