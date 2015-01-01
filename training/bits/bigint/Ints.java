public final class Ints {
  private Ints() {}


  public static void add(BitMap out, BitMap a, BitMap b) {
    int a_max = a.highest();
    int b_max = b.highest();
    int stop = Math.max(a_max, b_max);
    int carry = 0;
    for (int i = 0; i <= stop; i++) {
      if (a.get(i)) carry++;
      if (b.get(i)) carry++;
      out.set(i, (carry & 1) == 1);
      carry = carry >> 1;
    }
    if ((carry & 1) == 1) out.set(stop+1, true);
  }

  /** assumes that a > b */
  public static void minus(BitMap out, BitMap a, BitMap b) {
    int stop = b.highest();
    int carry = 0;
    for (int i = stop; i >= 0; i--) {
    }
  }

  public static void mult(BitMap out, BitMap a, BitMap b) {
    for (int i = 0; i <= b.highest(); i++) {
      if (b.get(i))
        add(out, out, a.shift(i));
    }
  }

 public static void main(String[] argv) {

    int x = Integer.parseInt(argv[0]);
    int y = Integer.parseInt(argv[1]);

    BitMap a = Bits.from(x);
    BitMap b = Bits.from(y);
    BitMap c = Bits.from(0);
    BitMap d = Bits.from(0);

    System.out.println(a);
    System.out.println(b);
    Ints.add(c,b,a);
    Ints.mult(d,b,a);
    System.out.println(c);
    //System.out.println(Integer.parseInt(c.toString(),2));
    System.out.println(d);
    //System.out.println(Integer.parseInt(d.toString(),2));

 }

}
