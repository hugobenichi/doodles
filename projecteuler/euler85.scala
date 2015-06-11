// We are going to get a recurrence relation for N(a,b) the number of triagle
// inside a rectangle of size (a,b).
//
// At one length a, all possible lengths i goes from 1 to a
// for i in [1:a], there is 1 + a - i positions.
//  example: (1,1): 1; (1, 2): 2; (2,3): 2, (1,3): 3
//
// therefore, the general formula is for N(a,b):
//
// N(a,b) = sum(i, [1,a]) sum(j, [1,b]) (1 + a - i) * (1 + b - j)
//
// Find recurence relation between N(a,b), N(a+1,b) and N(a, b+1):
//
// N(a+1,b) = sum(i, [1,a]) sum(j, [1,b]) (1 + a - (i-1)) * (1 + b - j)
//          = sum(i, [0,a-1]) sum(j, [1,b]) (1 + a -i) * (1 + b - j)
//          = N(a,b) - sum(j, [1,b]) (1 + a - a)(1 + b - j)
//                   + sum(j, [1,b]) (1 + a)(1 + b - j)
//          = N(a,b) + a sum(j, [1,b]) (1 + b - j)
//          = N(a,b) + a b (1 + b) - a sum(j, [1,b]) j
//          = N(a,b) + a b (1 + b) - a b (1 + b) / 2
//          = N(a,b) + a b (1 + b) / 2 // I am missing a factor b (1+b)/2 somewhere

def next(a: Int, b: Int, m: Map[(Int,Int),Int]) = {
  val x = m((a,b)) + (a + 1) * b * (b+1) / 2
  val y = m((a,b)) + (b + 1) * a * (a+1) / 2
  m + ((a+1, b) -> x) + ((a, b+1) -> y)
}

def update(a: Int)(m: Map[(Int,Int), Int], b: Int) = next(a,b,m)

def fillRow(m: Map[(Int,Int),Int], a: Int) = (1 to a).foldLeft(m)(update(a))

val seed = Map(((1,1) -> 1))

val m = (1 to 100).foldLeft(seed)(fillRow)

// sort this at the command line with $ scala euler85.scala | sort -n | head -1
m.toList foreach { t => Console println s"${(t._2 - 2e6).abs.toInt} ${t._1}" }
