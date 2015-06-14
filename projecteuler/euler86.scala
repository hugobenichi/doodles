// If the cuboid is "flattened", we notice the shortest path is a straight line
// and cn be expressed with Pythagore's theorem.
def path(a: Int, b: Int, c: Int) = (a+b) * (a+b) + c * c

def shortest(a: Int, b: Int, c: Int) =
  List(path(a,b,c), path(b,c,a), path(c,a,b)).min

def hasIntSolution(a: Int, b: Int, c: Int) = {
  val s = shortest(a,b,c)
  val d = math.sqrt(s).toInt
  d*d == s
}

def solutionsAt(a: Int): Int = {
  var n = 0
  for {
    b <- 1 to a
    c <- 1 to b
    if hasIntSolution(a,b,c)
  } { n += 1 }
  n
}

def find(n: Int): Int = {
  def loop(a: Int, tot: Int): Int = {
    val newtot = tot + solutionsAt(a)
    if (newtot > n) a else {
      Console println (a,newtot)
      loop(a+1, newtot)
    }
  }
  loop(1,0)
}

Console println find(1000000)
