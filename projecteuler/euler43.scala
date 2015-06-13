def isPandigital(s: String) = s.size == s.toSet.size

def isMult(n: Long)(m: Long) = m % n == 0

def mult3Digits(n: Long) =
  List.range[Long](1, 999) filter isMult(n) map (_.toString) filter isPandigital

def pad0(s: String): String = if (s.size < 3) pad0("0" + s) else s

def key(s: String) = s.subSequence(0,2).toString

val mults = List[Long](2,3,5,7,11,13,17) map mult3Digits map (_ map pad0)

val digits = (List range (0,9) map (_.toString)).toSet

def isChain(s: String, t: String) = s.substring(1,3) == t.substring(0,2)

def fuse(s: String, t: String) = s(0) + t

val m2 = mults(0)
val m3 = mults(1)
val m5 = mults(2)
val m7 = mults(3)
val m11 = mults(4)
val m13 = mults(5)
val m17 = mults(6)

// brute force works because contrains of chain of multiples yield few results.
// it is very ugly, I could regroup this in a recursive function or a fold
val chains = for {
  a <- m2
  b <- m3
  if isChain(a,b)
  c <- m5
  if isChain(b,c)
  d <- m7
  if isChain(c,d)
  e <- m11
  if isChain(d,e)
  f <- m13
  if isChain(e,f)
  g <- m17
  if isChain(f,g)
} yield a.substring(0,2) + b(1) + c(1) + d(1) + e(1) + f(1) + g.substring(1,3)

def complete(s: String) = (digits -- s.toList.map(_.toString)).toList.head + s

val all = chains filter isPandigital map complete

def sum(ls: List[String]) = (ls map (_.toLong)).sum

Console println sum(all)
