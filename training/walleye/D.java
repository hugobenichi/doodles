import java.math.BigInteger;

public class D {

  /*
    Question 1.1:

    The mathematical function D(n) where n is a non-negative integer is defined
    as follows:
      1. n < 10 => D(n) = n
      2. n >= 10 => D(n) = D(sum of the digits of n)

    Implement a java method that returns D(input) for n >= 0 and throws an
    illegal argument exception for n < 0

     We can do euclidean division by 10 on the input by and update a running sum
     with the remainder of the division, and repeat until we have consumed all
     digits of the input. This will cover both case n < 10 and n >= 10.

     An alternative solution would be to generate a string reprensentation of
     the BigInteger input and iterate over all chars in the sequence, updating
     the sum of digits by taking the ascii encoding difference with '0'.
  */

  private static final BigInteger ten = new BigInteger("10");

  public static int d(BigInteger input) {
    if (input.signum() < 0)
      throw new IllegalArgumentException("input was not a natural number");

    int sum = 0;
    while (input.signum() > 0) {
      BigInteger[] divMod = input.divideAndRemainder(ten);
      sum += divMod[1].intValue();
      input = divMod[0];
    }
    return sum;
  }

  /*
    Question 1.2

    What is D(314159265358979!)? (where ! indicates the factorial function)

    We can try to give an estimation method for computing D(n!). The idea is
    that with n sufficiently big, we can expect n! to have roughly an uniform
    distribution of digits in the range [0,9]. We can therefore estimate D(n!)
    by estimating the number of digits in n! and then multiplying by 4.5 the
    average of numbers in [0,9]. If we knew the exact probability distribution
    we could calculate a weighted average to get a more precise answer.

    To calculate the number of digits in n!, we use the fact that the number of
    digits in base b of a number x is 1 + log_b(x). The 1 offset covers x values
    below the base. By property of the log, we can extend this formula to
    products and get a direct way to estimate D(n!) which is much cheaper than
    calculating the factorial.

    However, this will not scale up to 314159265358979. Instead of computing the
    sum, we can use Stirling's approximation of ln(n!) to obtain:

      D(n!) approx 1 + n (ln(n)/ln(10) - 1)

    For n = 314159265358979, this formula gives D(n!) = 1.908114609800532E16

  */

  /** Exact answer */
  public static int dfact(int n) {
    BigInteger r = new BigInteger("1");
    while (n > 0) {
      r = r.multiply(BigInteger.valueOf(n--));
    }
    return d(r);
  }

  /** First approximation */
  public static double estimateDFactWithSum(int n) {
    double sum = 0.0;
    while (n > 0) {
      sum += Math.log10(n--);
    }
    return 4.5 * sum;
  }

  /** Second approximation */
  public static double estimateDFactWithStirling(BigInteger n) {
    double x = n.doubleValue();
    return 4.5 * (1 + x * ( Math.log(x) / Math.log(10) - 1));
  }

  public static void main(String[] argv) {
  /*
    assert 3 == d(new BigInteger("3"));
    assert 6 == d(new BigInteger("6"));
    assert 1 == d(new BigInteger("10"));
    assert 7 == d(new BigInteger("16"));
    assert 14 == d(new BigInteger("2345"));
    assert 6 == d(new BigInteger("111111"));
    assert 24 == d(new BigInteger("111111111111111111111111"));
  */
  /*
    BigInteger m = new BigInteger("314159265358979");
    System.out.println(estimateDFactWithStirling(m));
  */
    int n = Integer.parseInt(argv[0]);
    System.out.println(dfact(n));
    System.out.println(estimateDFactWithSum(n));
    System.out.println(estimateDFactWithStirling(BigInteger.valueOf(n)));
  }
}
