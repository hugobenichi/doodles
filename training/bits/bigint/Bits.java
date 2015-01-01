import java.util.Arrays;

public class Bits implements BitMap {

  public static class ShiftedBitMap implements BitMap {
    private final BitMap back;
    private final int shift;

    public ShiftedBitMap(BitMap back, int shift) {
      this.back = back.copy();
      this.shift = shift;
    }

    public void set(int pos, boolean val) { back.set(pos-shift, val); }

    public boolean get(int pos) { return back.get(pos - shift); }

    public int highest() {
      int shifted_highest = back.highest() + shift;
      return shifted_highest > 0 ? shifted_highest : 0;
    }

    public BitMap shift(int bits) { return new ShiftedBitMap(back, bits+shift); }

    public BitMap copy() { return new ShiftedBitMap(back, shift); }

    public String toString() { return Bits.stringOf(this); }
  }

  private boolean[] ary;
  private int highest;

  private Bits() {
    this.highest = 0;
    this.ary = new boolean[10];
  }

  private Bits(boolean[] ary, int highest) {
    this.highest = highest;
    this.ary = Arrays.copyOf(ary, highest+1);
  }

  public void set(int pos, boolean val) {
    if (val) setUp(pos); else setDown(pos);
  }

  private void setUp(int pos) {
    if (pos < 0) return;

    if (pos >= ary.length)
      ary = Arrays.copyOf(ary, Math.max(pos+1, ary.length * 2));

    if (pos > highest)
      highest = pos;

    assert ary.length > highest;
    assert ary.length > pos;
    ary[pos] = true;
  }

  private void setDown(int pos) { if (inRange(pos)) ary[pos] = false; }

  public boolean get(int pos) {
    //System.out.println("get on " + pos + ": range = " + inRange(pos)
                     //+ "| highest: " + highest + " | len: " + ary.length);
    if (inRange(pos)) return ary[pos]; else return false;
  }

  private boolean inRange(int pos) {
    assert ary.length > highest;
    assert pos < ary.length;
    return 0 <= pos && pos <= highest;
  }

  public int highest() { return highest; }

  public BitMap shift(int bits) { return new ShiftedBitMap(this, bits); }

  public BitMap copy() { return new Bits(ary, highest); }

  public BitMap blank() { return new Bits(); }

  public String toString() { return Bits.stringOf(this); }

  public static BitMap from(int val) {
    BitMap bm = new Bits();
    if (val < 0) return bm;
    int pos = 0;
    while (val != 0) {
      if ((val & 1) == 1) bm.set(pos, true);
      pos++;
      val = val >> 1;
    }
    return bm;
  }

  public static String stringOf(BitMap bm) {
    StringBuilder bld = new StringBuilder();
    for (int i = bm.highest(); i >= 0; i--) {
      bld.append(bm.get(i) ? '1' : '0');
    }
    return bld.toString();
  }

}
