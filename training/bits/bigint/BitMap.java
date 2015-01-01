public interface BitMap {

  void set(int pos, boolean value);
  boolean get(int pos);
  int highest();
  BitMap shift(int bits);
  BitMap copy();

}
