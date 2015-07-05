def digSum(x: BigInt) = x.toString.toList.map(_ - '0').sum

val sums = for {
  a <- (1 to 100) map (BigInt(_))
  b <- (1 to 100)
} yield digSum(a pow b)

println(sums.max)
