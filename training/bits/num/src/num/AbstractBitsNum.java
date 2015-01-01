package num;

import num.Num;
import num.Nums;

abstract class AbstractBitsNum implements Num {

    public abstract int[] bits();

    public abstract boolean isPos();
    public abstract boolean isNeg();
    public abstract Num inv();
    public abstract Num abs();

    public abstract int compareTo(Num other);
    public abstract Num plus(Num other);
    public abstract Num min(Num other);
    public abstract Num mult(Num other);
    public abstract Num div(Num other);

    public String bitstring() { return Nums.bitstring(this.bits()); }
    public String decstring() { return Nums.decstring(this.bits()); }
}
