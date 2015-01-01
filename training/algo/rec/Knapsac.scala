object Knapsack {

  type Sol = Array[(Int,Int)]

  def considerNewItem(cap: Int, prior: Sol, v: Int, w: Int): (Int, Sol) = {
    if (w > cap) return prior
    val newSol = new Array[(Int,Int)](cap+1)
    newSol(0) = 0
    (0 until cap) foreach {
      k =>
        val candidateValue =
          if (k-w < 0) prior(k) else prior(k) max (v + prior(k-w))
          next = 
    }
    return newSol
  }


  def main(argv: Array[String]) {

    Console println "foo"
  }
}
