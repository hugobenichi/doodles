import java.util.Random;
import java.util.Arrays;

public class Sum {

  private static final Random r = new Random();

  private static int get(int upto) { return r.nextInt() % upto; }

  public static int quad(int[] ary) {
    int sum = 0;
    int best = 0;
    for (int i = 0; i < ary.length; i++) {
      sum = 0;
      for (int j = i; j < ary.length; j++) {
        sum += ary[j];
        if (sum > best)
          best = sum;
      }
    }
    return best;
  }

  public static int partial(int[] ary) {
    int psum = 0;
    int best = ary[0];
    int lowest = 0;
    for (int i = 0; i < ary.length; i++) {
      psum += ary[i];
      if (psum - lowest > best) {
        best = psum - lowest;
      }
      if (psum < lowest) {
        lowest = psum;
      }
    }
    return best;
  }

  public static int solution(int[] ary) {
    int sum = 0;
    int best = sum;
    for (int i = 0; i < ary.length; i++) {
      sum += ary[i];
      if (sum > best) best = sum;
      if (sum < 0) sum = 0;
    }
    return best;
  }

  public static void printAry(int[] ary) {
    for (int i : ary) { System.out.print(i + " "); }
    System.out.println();
  }

  public static void main(String[] argv) {
    int[] x = new int[]{-5,0,-1,8,-5,2,-8,-3,-1,9};
    int[] y = new int[]{-5,0,5,-5,0,-5,4,-1,3,3};
    int[] ary = new int[Integer.parseInt(argv[0])];
    for (int i = 0; i < ary.length; i++) { ary[i] = get(10); }
    //ary = x;
    printAry(ary);
    System.out.println(solution(ary));
    System.out.println(quad(ary));
    System.out.println(partial(ary));
  }
}
