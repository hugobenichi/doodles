import scala.collection.mutable.ListBuffer

def triangle(n: Long): Long = n * (n+1) / 2
def penta(n: Long): Long = n * (3*n-1) / 2
def hexa(n: Long): Long = n * (2*n - 1)

// brute force eager eval
val nats = (1 to 1000000 map {x:Int => x.toLong}).toList
val trias = nats map triangle
val pents = nats map penta
val hexas = nats map hexa

def inter(l1: List[Long], l2: List[Long], buf: ListBuffer[Long] = ListBuffer[Long]()): List[Long] =
  (l1,l2) match {
    case (_, Nil) => buf.toList
    case (Nil, _) => buf.toList
    case (x1 :: t1, x2 :: t2) =>
      var n1 = t1
      var n2 = t2
      if (x1 == x2) {
        buf += x1
      } else if (x1 < x2) {
        n2 = l2
      } else {
        n1 = l1
      }
      inter(n1,n2,buf)
  }

inter(inter(trias, pents), hexas) foreach println
