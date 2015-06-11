import scala.math.BigDecimal

// We are going to use Newton's method to compute a square root.
// Newtown's method find the zero of a function f(x) with the iterative formula
//
//   x_{n+1} = x_n - f(x_n) / f'(x_n)
//
// If we look for the square root b of a, we have b * 2 = a. A function whose zero
// is b is:
//
//   f(x) = x^2 - a
//
// which gives the iterative formula
//
//   x_{n+1} = x_n - (x_n^2 -a) / 2 x_n = x_n / 2 + a / 2 x_n

val ctx = new java.math.MathContext(110)

def sqrt(a: BigDecimal): BigDecimal = {
  def loop(x: BigDecimal, n: Int): BigDecimal =
    if (n == 0) x else loop((x + a / x)/2, n-1)
  loop(BigDecimal(1, ctx), 1000)
}

val squares = Set(1,4,9,16,25,36,49,64,81,100)
def notSq(n: Int) = !squares(n)

def sqrtDigits(n: Int) = sqrt(BigDecimal(n, ctx)).toString

def sumDigits(s: String) =
  s.toList.filter( _ != '.').take(100).map(_.toString.toInt).sum

Console println (1 to 100 filter notSq map sqrtDigits map sumDigits).sum
