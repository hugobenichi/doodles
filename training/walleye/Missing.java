import java.util.Random;
import java.util.Arrays;

/*
  A well-known question is stated as follows:
  Given an input array of size N-1 containing all the integers between 1 and N
  except a single integer that is missing, find the missing number. Several
  efficient solutions exist. For example: Sum all the numbers, then subtract
  the sum from N*(N+1)/2. Note that this solution only requires allocating
  O(log N) bits of memory to store the sum. A slightly more interesting version
  of the problem is that K different integers are missing.
*/
public class Missing {

  public static void main(String[] argv) {
    int n = 100;
    int m = 2;
    int[] input = prepareAry(n,m);
    //Missing.printMissing(input);
    TwoMissing.printMissing(input);
    KMissing.printMissing(Arrays.copyOf(input, n));
  }

  public static int[] prepareAry(int len, int missing) {
    int[] ary = new int[len + missing];
    for (int i = 0; i < len + missing; i++) {
      ary[i] = i + 1;
    }
    shuffle(ary);
    return Arrays.copyOf(ary, len);
  }

  public static void shuffle(int[] ary) {
    Random r = new Random();
    for (int i = 0; i < ary.length; i++) {
      int idx = r.nextInt(ary.length - i);
      int temp = ary[i];
      ary[i] = ary[i + idx];
      ary[i + idx] = temp;
    }
  }

  public static void printMissing(int[] ary) {
    int N = ary.length + 1;
    int sum = N * (N + 1)/2;
    for (int i = 0; i < ary.length; i++) {
      sum -= ary[i];
    }
    System.out.println(sum);
  }
}

/*
  Question 3.1
  Write a java method that will only require O(log N) bits of memory to find
  the missing integers in the case K=2 and print them to System.out.

  To solve the special case of two missing numbers, we can try to build up a
  solution from the one missing number case. Let's assume only one number is
  missing and let's call g the result of the simple algorithm comparing the sum
  of the numbers in the array to the expected sum N(N+1)/2. If n and m and the
  two missing numbers, we have the relation:
    n + m = g
  Furthermore, by definition we know that n != m, so let's assume n < m. Because
  the above sum relation must hold, we can deduce that n and m are always
  seperated by half the value of g. If g is even then we have
    0 < n < g/2 < m <= N
  and if g is odd, we have
    0 < n <= g/2 < m <= N
  We see once again the problem of finding one missing number n in the range
  [1,g/2] and from these relations we can adopt the following strategy for
  finding n and m:
    - calculate g as if only one number was missing from the array.
    - calculate the sum of all numbers in the array that are also less or
      equal to g/2.
    - n is the difference between (g/2) * (g/2 - 1) / 2 and this second sum.
    - m is the difference between g and n.

  The solution matches the space requirement because it does not do any
  allocations besides the int variables on the stack, which takes at most
  log(N) bits to represent the largest possible number we can encounter: N.
*/
class TwoMissing {
  public static void printMissing(int [] input) {
    int N = input.length + 2;
    int gap = N * (N + 1) / 2;
    for (int i = 0; i < input.length; i++) {
      gap -= input[i];
    }
    int hgap = gap/2;
    int n = hgap * (hgap + 1) / 2;
    for (int i = 0; i < input.length; i++) {
      int x = input[i];
      if (x <= hgap) {
        n -= x;
      }
    }
    System.out.println(n);
    System.out.println(gap - n);
  }
}

/*
  Question 3.2
  Write a java method that will only require O(K log N) bits of memory to find
  the K missing integers in the general case and print them to System.out.

  The simplest solution would be to sort the array, then traverse it and print
  the missing numbers as we detect them. To detect missing numbers, we check
  if the number at index i in the sorted array is equal to the index + 1, and we
  subtract the number of missing numbers found until there, to compensate for
  the offset in the index. This method will automatically detect K by not
  printing numbers which are missing at the end of the array.

  If we use the system sort, this solution will certainly not meet the space
  requirements, since we can expect the system sort to have a O(N log N) time
  complexity using log N stack space, which each call frame taking log N space
  for local variables. If there is no strong time complexity requirement, we
  can improve the space complexity by using a O(N^2) sorting algorithm that
  would not consume any stack space.
*/
class KMissing {
  public static void printMissing(int[] input) {
      Arrays.sort(input);
      int missing = 1;
      int i = 0;
      while (i < input.length) {
        if (input[i] != i + missing) {
          System.out.println(i + missing);
          missing++;
        } else {
          i++;
        }
      }
  }
}
