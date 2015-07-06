import scala.collection.mutable.ListBuffer
import scala.io.Source
import scala.annotation.tailrec

val file = "p059_cipher.txt"
val coded = Source.fromFile(file).getLines.next.split(',').map(_.toInt).toList

def join(ls: List[Int]): String = (ls map (_.toChar)).mkString

def zor(ls: List[Int], k: List[Int]): List[Int] = {
  @tailrec
  def loop(ls: List[Int], ks: List[Int], qs: ListBuffer[Int]): List[Int] =
    (ls, ks) match {
      case (Nil, _) => qs.toList
      case (_, Nil) => loop(ls, k, qs)
      case (h :: t, ks :: kt) => loop(t, kt, qs += h ^ ks)
    }
  loop(ls, k, ListBuffer[Int]())
}

// we guess the code by printing codes for every 3*n, 3*n+1, and 3*n+2 code
// points and finding the most common values. These can either be a frequen
// English letter like 'e', 'a' or 't', or space. We then validate by applying
// the guess to a full subsequence et checking we get mostly letters.
val a = ' '.toInt ^ 71
val b = ' '.toInt ^ 79
val c = ' '.toInt ^ 68

val ks = List(a,b,c)

val decoded = zor(coded,ks)
println(join(decoded))

println(decoded.sum)

//val sub = decoded.zipWithIndex.collect { case (e,i) if i % 3 == 2 => e }
//println(join(sub))

//coded foreach println
//coded.zipWithIndex.collect { case (e,i) if i % 3 == 2 => e } foreach println
