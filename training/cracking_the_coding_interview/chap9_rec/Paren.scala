/*

num of pairs remaining
num of left par
seq of Char

*/
object Paren {

    def fill(n: Int, pars: List[Char]): List[Char] =
        if (n == 0) pars else fill(n-1, '(' :: pars)

    def parToString(pars: List[Char]): String = pars.foldLeft("") {_ + _}

    def perm(pairs: Int, right: Int, pars: List[Char]) {
        if (right > pairs) return
        if (pairs == 0) {
            Console println parToString(pars)
            return
        }
        if (right > 0) perm(pairs - 1, right - 1, '(' :: pars)
        perm(pairs - 1, right + 1, ')' :: pars)
    }

    def perm2(pairs: Int, right: Int, pars: List[Char], res: List[String])
    : List[String] = {
        if (right > pairs) return res
        if (pairs == 0)
            return parToString(pars) :: res
        val ress = perm2(pairs - 1, right + 1, ')' :: pars, res)
        if (right > 0)
            return perm2(pairs - 1, right - 1, '(' :: pars, ress)
        else
            return ress
    }

    def permut(pairs: Int, index: Int, buf: Array[Char]): Unit =
        if (pairs == 0) {
            Console println buf.foldLeft(""){_ + _}
        } else {
            val nestedPairs = pairs - 1
            buf(index) = '('
            buf(index + 1) = ')'
            permut(nestedPairs, index+2, buf)
            if (pairs > 1) {
                buf(index + 1 + 2*nestedPairs) = ')'
                permut(nestedPairs, index+1, buf)
                /*
                buf(2*nestedPairs + index) = '('
                buf(2*nestedPairs + index + 1) = ')'
                permut(nestedPairs, index, buf)
                */
            }
        }

    def main(argv: Array[String]) {
        val pairs = argv(0).toInt
        permut(pairs, 0, new Array[Char](2*pairs))
        Console println ""
        perm(2*pairs, 0, Nil)
    }

}
