case class Rat(n: BigInt, d: BigInt) {
  def next: Rat = Rat(d, 2*d+n)
  def last: Rat = Rat(n+d,d)
  def count = n.toString.length > d.toString.length
}

def seq(r: Rat): Stream[Rat] = r.last #:: seq(r.next)

val root = Rat(BigInt(1),BigInt(2))

Console println (seq(root) take 1000 filter (_.count)).size
