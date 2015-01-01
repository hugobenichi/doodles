import scala.collection.mutable

object Coins {

  def main(argv: Array[String]) {
    val start = System.nanoTime
    val num = coins(argv(0).toInt)
    val end = System.nanoTime
    val elap = (end - start) / 1000000
    Console println num + " possibilities"
    Console println elap + " ms"
  }

  def coins(n: Int): Int = {
    val coins = 25 :: 10 :: 5 :: 1 :: Nil
    rec(n, coins, mutable.Map[(Int,List[Int]),Int]())
    //rec1(n, coins)
  }

  def rec1(n: Int, coins: List[Int]): Int =
    if (coins.tail.isEmpty) {
      if (n % coins.head == 0) 1 else 0
    } else {
      val others = rec1(n, coins.tail)
      val c = coins.head
      if (n-c == 0) return 1 + others
      if (n-c < 0) return others
      others + rec1(n-c, coins)
    }

  def rec(n: Int, coins: List[Int], cache: mutable.Map[(Int,List[Int]),Int]): Int =
    if (coins.tail.isEmpty) {
      if (n % coins.head == 0) 1 else 0
    } else {
      val others = ensureCache(n, coins.tail, cache)
      val c = coins.head
      if (n-c == 0) return 1 + others
      if (n-c < 0) return others
      others + ensureCache(n-c, coins, cache)
    }

  def ensureCache(n: Int, coins: List[Int],
                  cache: mutable.Map[(Int,List[Int]),Int]): Int = {
    val key = (n,coins)
    val fromCache = cache get key
    if (fromCache.isDefined) fromCache.get else {
       val num = rec(n, coins, cache)
       cache += Tuple2(key,num)
       num
     }
  }

}
