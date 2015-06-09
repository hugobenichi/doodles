def primes(ls: List[Int]): List[Int] = ls match {
  case Nil => Nil
  case h :: t => h :: primes(t filter { n => (n % h) != 0 })
}

// generates all primes we are interested in
val ls = primes((2 to 10000).toList) filter { x => x > 1000 }

// group by permutation groups, and remove groups of size 1 and 2
val sets = ls groupBy { _.toString.toSet } filter { tup => tup._2.size > 2 }

// group a list of primes in the same permutation group into List of
// pairs with the same difference.
def groupByDiff(ls: List[Int]): List[List[(Int,Int)]] = {
  val pairs = for {
    a <- ls
    b <- ls if b < a
  } yield (b, a)
  def diff(t: (Int,Int)):Int = t._2 - t._1
  (pairs groupBy diff filter { t => t._2.size > 1 }).values.toList
}

// find pairs of pairs that form a triplets of consecutive diffs.
def sameConsecutiveDiff(ls: List[(Int,Int)]): List[(Int,Int,Int)] = ls match {
  case Nil => Nil
  case h :: t => Nil
    def isNext(tup: (Int,Int)) = tup._1 == h._2
    def triplet(tup: (Int,Int)) = (h._1, h._2, tup._2)
    (t filter isNext map triplet) ++ sameConsecutiveDiff(t)
}

def solutions(ls: List[Int]) = groupByDiff(ls) flatMap sameConsecutiveDiff

sets.values flatMap solutions foreach println
