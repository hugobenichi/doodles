public class Digit {

  public static void main(String[] argv) {
      for (int i = 1; i < 12; i++) {
        for (int j = 1; j < 12; j++) {
          double d = Math.log10(i);
          double e = Math.log10(j);
          System.out.println(i + " x " + j + " = " + (i * j) + " -> " + (1 + d + e));
        }
      }

      double sum = 0.0;
      for (int i = 1; i < 1000; i++) {
        sum += Math.log10(i);
      }
      System.out.println(4.5 * sum);
  }

}
