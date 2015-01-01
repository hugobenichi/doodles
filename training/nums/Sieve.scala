object Sieve {

  def naturals(n: Int = 0): Stream[Int] = n #:: naturals(n+1)

  def primes(nums: Stream[Int] = naturals(2)): Stream[Int] =
    nums.head #:: primes(nums.tail filter { _ % nums.head != 0 })

  def main(args: Array[String]) {
    Console println (primes() take args(0).toInt).toList 
  }

}
