abstract public class AInt implements Int<AInt> {

  private final boolean sign = true;
  private final BitMap bits = null;

  public boolean isPos() { return sign; }
  public boolean isNeg() { return !isPos(); }

  public AInt plus(AInt that) {
    return this;
  }

  public AInt minus(AInt that) { return this; }
  public AInt mult(AInt that) { return this; }
  public AInt div(AInt that) { return this; }

  public static Int of(int x) { return new PosInt(); }

  private static class PosInt extends AInt {

    public boolean isPos() { return true; }

    protected void setBit(long pos) {}
    protected void unsetBit(long pos) {}
    protected boolean getBit(long pos) { return true; }
    protected long highest() { return 0; }

  }

}
