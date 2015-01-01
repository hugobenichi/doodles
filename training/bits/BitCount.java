public class BitCount {

  private static final int[] bitsOfAByteLup = bitsLup();

  public static int totalBits(Iterator<Integer> ints) {
    int total = 0;
    for (int i : ints) {
      total += bitsNum(i);
    }
    return total;
  }

  public static int bitsNum(int i) {
    return bitsOfAByteLup[i & 0xff]
         + bitsOfAByteLup[(i >> 8) & 0xff]
         + bitsOfAByteLup[(i >> 16) & 0xff]
         + bitsOfAByteLup[(i >> 24) & 0xff];
  }

  public static int[] bitsLup() {
    int len = 1 << 8;
    int[] lup = new int[len];

    for (int i = 0; i < len; i++) {
      lup[i] = Integer.bitCount(i);
    }
    return lup;
  }

}
