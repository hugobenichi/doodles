import scala.math.BigInt

def fact(x: BigInt, n: Int): Stream[BigInt] = x #:: fact(x*n, n+1)

val f = (fact(BigInt(1), 1) take 101).toArray

def binomial(n: Int)(r: Int) = f(n) / f(r) / f(n-r)

def binomials(n: Int): List[BigInt] = List range (0,n+1) map binomial(n)

val all = List range (0,101) map binomials

def oneM(b: BigInt): Boolean = b > 1000000

//Console println (all filter (_ > 1000000)).size

Console println all.flatten.filter(oneM).size
