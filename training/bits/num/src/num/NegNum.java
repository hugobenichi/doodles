package num;

import num.Arithmetics;

class NegNum extends AbstractBitsNum {

    public final int[] bits;

    public NegNum(int[] bits) { this.bits = bits; }
    public int[] bits() { return bits; }
    public boolean isPos() { return false; }
    public boolean isNeg() { return true; }
    public Num inv() { return new PosNum(bits); } // cache this
    public Num abs() { return this.inv(); }    

    /* x < y <=> -x > -y */
    public int compareTo(Num other) { return -this.compareTo(other.inv()); }

    /* x + y = -( (-x) + (-y) ) */
    public Num plus(Num other) { return this.inv().plus(other.inv()).inv(); }

    /* x - y = -( (-x) + y ) */
    public Num min(Num other) { return this.inv().plus(other).inv(); }

    /* x * y = -( (-x) * y ) */
    public Num mult(Num other) { return this.inv().mult(other).inv(); }

    public Num div(Num other) { return this.inv().div(other).inv(); }

}
