import scala.io.Source

def split(s: String): List[Int] = (s split "," map (_.toInt)).toList

//val file = "minimat"
val file = "p082_matrix.txt"

val matls = (Source fromFile file).getLines.toList map split

val lim = matls.length - 1


type Cell = (Int,Int)

def cell(n: Int)(tup: Cell) = ((n,tup._2), tup._1)

def rowCells(tup: (List[Int], Int)) = tup._1.zipWithIndex map cell(tup._2)

def toCells(mat: List[List[Int]]) = mat.zipWithIndex flatMap rowCells

val mat = toCells(matls).toMap



def right(c: Cell) = (c._1, c._2 + 1)
def down(c: Cell) = (c._1 - 1, c._2)
def up(c: Cell) = (c._1 + 1, c._2)

def add(m: Map[Cell,Int], c: Cell) = m + (c -> (mat(c) + m.getOrElse(right(c), 0)))

def init(m: Map[Cell,Int], j: Int) = add(m, (j, lim))

val seed = (lim to 0 by -1).foldLeft(Map.empty[Cell,Int])(init)


def column(j: Int) = (lim to 0 by -1) map ((_, j))

def score(m: Map[Cell, Int], from: Cell)(to: Cell): Int = {
  def loop(from: Cell, to: Cell, base: Int): Int =
    if (from._1 == to._1)
      base + mat(from)
    else if (from._1 < to._1)
      loop(up(from), to, base + mat(from))
    else
      loop(down(from), to, base + mat(from))
 loop(from, to, m(to))
}

def bestScore(m: Map[Cell, Int], f: Cell) = (column(f._2+1) map score(m,f)).min

def updateScore(m: Map[Cell, Int], f: Cell) = m + (f -> bestScore(m, f))

def update(m: Map[Cell,Int], j: Int) = column(j).foldLeft(m)(updateScore)

val agg = (lim - 1 to 0 by -1).foldLeft(seed)(update)

Console println (column(0) map (agg(_))).min
