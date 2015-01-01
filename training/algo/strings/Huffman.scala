import scala.collection.mutable.ListBuffer
import scala.collection.mutable.PriorityQueue
import scala.collection.mutable.Queue

object Huffman {

  type Count = Map[Char,Int]

  def stringCharCounts(s: String, counts: Count) = s.toList.foldLeft(counts) {
    case (acc, c) => acc + Tuple2(c, acc.getOrElse(c, 0) + 1)
  }

  def charCount(text: Seq[String], counts: Count = Map[Char,Int]()): Count =
    text match {
      case Nil        => counts
      case s :: tail  => charCount(tail, stringCharCounts(s, counts))
    }

  def newPQueue = new PriorityQueue[CodeTree]()(CodeTree.ordering)

  def orderNodes(counts: Count) =
    counts.foldLeft(newPQueue) { case (pq, (c,freq)) => pq += Leaf(c,freq) }

  def makeTree2(text: Seq[String]): CodeTree = {
    val leafs = orderNodes(charCount(text))
    val codes = Queue[CodeTree]()
    def dequeueSmallest(): CodeTree = {
      if (leafs.isEmpty) return codes.dequeue
      if (codes.isEmpty) return leafs.dequeue
      if (leafs.head.freq < codes.head.freq) leafs.dequeue else codes.dequeue
    }
    while (leafs.nonEmpty || codes.size > 1) {
      val right = dequeueSmallest()
      val left = dequeueSmallest()
      codes += left + right
    }
    codes.head
  }
  def makeTree(text: Seq[String]): CodeTree = {
    val codes = orderNodes(charCount(text))
    while (codes.size > 1) {
      val right = codes.dequeue
      val left = codes.dequeue
      codes += left + right
    }
    codes.head
  }

  trait CodeTree {

    def freq: Int
    def chars: Set[Char]
    def get(codedSeq: Seq[Boolean]): (Char, Seq[Boolean])
    def gather(path: List[Boolean]): List[(Char, List[Boolean])]

    def +(that: CodeTree): CodeTree = that match {
      case Leaf(c, f) => Fork(that, this, this.chars + c, this.freq + f)
      case _ => Fork(this, that, this.chars ++ that.chars, this.freq + that.freq)
    }


    def decoder: Seq[Boolean] => String = {
      def traverse(codedSeq: Seq[Boolean], msg: ListBuffer[Char]): String = {
        val (char, tail) = this get codedSeq
Console println s"got $char"
Console println s"  from $codedSeq"
Console println s"  left $tail"
        msg += char
        if (tail.isEmpty) msg mkString "" else traverse(tail, msg)
      }
      traverse(_, ListBuffer[Char]())
    }

    def encoder: String => Seq[Boolean] = {
      val table = gather(Nil).foldLeft(Map[Char,Seq[Boolean]]()) {
        case (acc, (char, seq)) => acc + (char -> seq.reverse)
      }
Console println table
      def fromTable(s: String, table: Map[Char,Seq[Boolean]]): Seq[Boolean] =
        s.toList.flatMap { case c => table.getOrElse(c,Nil) }
      fromTable(_, table)
    }

  }

  object CodeTree {
    implicit val ordering: Ordering[CodeTree] = new Ordering[CodeTree]() {
      override def compare(a: CodeTree, b: CodeTree) = b.freq compare a.freq
    }
  }

  case class Fork(left: CodeTree, right: CodeTree,
                  chars: Set[Char], freq: Int) extends CodeTree {
    override def toString(): String = {
      val charSet = chars mkString ","
      s"($freq,{$charSet}) -> [$left, $right]"
    }
    override def get(codedSeq: Seq[Boolean]) = {
      (if (codedSeq.head) right else left) get codedSeq.tail
    }
    override def gather(path: List[Boolean]) =
      (left gather false :: path) ++ (right gather true :: path)
  }

  case class Leaf(char: Char, freq: Int) extends CodeTree {
    override def chars = Set(char)
    override def toString(): String = s"($freq,$char)"
    override def get(codedSeq: Seq[Boolean]) = (char, codedSeq)
    override def gather(path: List[Boolean]) = (char, path) :: Nil
  }

  object Leaf { def apply(args: (Char,Int)): CodeTree = Leaf(args._1, args._2) }

  def main(args: Array[String]) {
    val code = makeTree2(Seq(args(0)))
Console println code
    val enc = code.encoder
    val dec = code.decoder
    val msg = enc(args(1))
Console println msg
    val bck = dec(msg)
Console println s"$bck -> ${bck == args(1)}"
  }

}
