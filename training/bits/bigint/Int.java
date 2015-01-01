public interface Int<Self extends Int> {

  boolean isPos();
  boolean isNeg();
  Self plus(Self that);
  Self minus(Self that);
  Self mult(Self that);
  Self div(Self that);

}
