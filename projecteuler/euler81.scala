import scala.io.Source

def split(s: String): List[Int] = (s split "," map (_.toInt)).toList

val file = "p081_matrix.txt"

val matls = (Source fromFile file).getLines.toList map split



type Cell = (Int,Int)

def cell(n: Int)(tup: Cell) = ((n,tup._2), tup._1)

def rowCells(tup: (List[Int], Int)) = tup._1.zipWithIndex map cell(tup._2)

def toCells(mat: List[List[Int]]) = mat.zipWithIndex flatMap rowCells

val mat = toCells(matls).toMap



def right(c: Cell): Cell = (c._1, c._2 + 1)
def down(c: Cell): Cell = (c._1 + 1, c._2)

def update(m: Map[Cell,Int], c: Cell): Map[Cell,Int] = {
  val score = (m get down(c), m get right(c)) match {
    case (Some(s), None) => s
    case (None, Some(s)) => s
    case (Some(s1), Some(s2)) => s1 min s2
    case (None, None) => throw new Exception // cannot be both None
  }
  m + (c -> (score + mat(c)))
}

val lim = 79

// iterate from bottom to top, from right to left
val cells = for {
  i <- lim to 0 by -1
  j <- lim to 0 by -1
  if i != j || i != lim
} yield (i,j)

val root = (0,0)
val end = (lim,lim)
val agg = cells.foldLeft(Map(end -> mat(end)))(update)

Console println agg(root)
