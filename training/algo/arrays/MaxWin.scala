class MaxWin(val width: Int) {

  require(width > 0)

  private val nums: Array[Int] = Array.tabulate(width) { _ => Int.MinValue }
  private var idx: Int = 0
  private var max: Int = Int.MinValue
  private var maxIdx: Int = 0

  def push(x: Int): Unit = {
    nums(idx) = x
    if (x >= max) {
      max = x
      maxIdx = idx
    } else if (idx == maxIdx) {
      findNewMax()
    }
    incrementIdx()
  }

  def currentMax: Int = max

  private def findNewMax(): Unit = {
    var i = 0
    max = Int.MinValue
    while (i < nums.length) {
      val x = nums(i)
      if (x >= max) {
        max = x
        maxIdx = i
      }
      i += 1
    }
  }

  private def incrementIdx() = {
    idx += 1
    if (idx == nums.length) idx = 0
  }

}

object MaxWin {

  def main(argv: Array[String]) {
    Seq[Seq[Int]](
      List.tabulate(10) { _ => 3 },
      List.tabulate(10) { x => 1 - (x % 2)*2 },
      List.tabulate(10) { x => x},
      List.tabulate(10) { x => 10 - x }
    ) foreach {
      stream =>
        val win = new MaxWin(4)
        stream foreach {
          x =>
            win push x
            Console println win.currentMax
        }
    }
  }

}
