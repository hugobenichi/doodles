object StairCase {

    def countSteps(n: Int): Int = {
        if (n < 0) return 0;
        if (n == 0) return 1;
        return countSteps(n-3) + countSteps(n-2) + countSteps(n-1);
    }

    def countStepsFast(n: Int, cache: Array[Int]): Int = {
        if (n < 0) return 0;
        if (n == 0) return 1;
        return tryCache(n-3, cache) + tryCache(n-2, cache) + tryCache(n-1, cache)
    }

    def tryCache(n: Int, cache: Array[Int]): Int = {
        if (n < 0) return 0;
        if (n == 0) return 1;
        if (cache(n) != 0) return cache(n)
        val steps = countStepsFast(n, cache)
        cache(n) = steps
        return steps
    }


    def main(argv: Array[String]) {
        val n = argv(0).toInt

        val t1 = System nanoTime()
        val a = countSteps(n)
        val t2 = System nanoTime()
        Console println a
        Console println t2 - t1

        val t3 = System nanoTime()
        val b = countStepsFast(n, new Array[Int](n))
        val t4 = System nanoTime()
        Console println b
        Console println t4 - t3
    }

}
