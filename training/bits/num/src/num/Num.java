package num;

public interface Num extends Comparable<Num>{
    String bitstring();
    String decstring();
    boolean isPos();
    boolean isNeg();
    Num plus(Num other);
    Num min(Num other);
    Num mult(Num other);
    Num div(Num other);
    Num inv();
    Num abs();
}
