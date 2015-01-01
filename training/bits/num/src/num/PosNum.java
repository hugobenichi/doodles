package num;

import num.Arithmetics;
import num.Nums;

class PosNum extends AbstractBitsNum {

    private final int[] bits;

    public PosNum(int[] bits) { this.bits = bits; }
    public int[] bits() { return bits; }
    public boolean isPos() { return true; }
    public boolean isNeg() { return false; }
    public Num inv() { return new NegNum(bits); } // cache this
    public Num abs() { return this; }    

    public int compareTo(Num other) {
        int[] x = this.bits;
        int[] y = Nums.bitsOf(other);
        if (other.isNeg()) return +1;
        return Arithmetics.compare(x, y);
    }

    public Num plus(Num other) {
        int[] x = this.bits;
        int[] y = Nums.bitsOf(other);
        if (other.isPos())
            return new PosNum( Arithmetics.plus(x,y) );
        if (this.compareTo(other.abs()) > 0)
            return new PosNum( Arithmetics.minus(x,y) );
        else
            return new NegNum( Arithmetics.minus(y,x) );
    }

    /* x - y = x + (-y) */
    public Num min(Num other) {
        return this.plus(other.inv());
    }

    public Num mult(Num other) {
        int[] x = this.bits;
        int[] y = Nums.bitsOf(other);
        Num n = new PosNum(Arithmetics.multiply(x, y));
        if (other.isNeg())
            n = n.inv();
        return n;
    }

    public Num div(Num other) {
        int[] x = this.bits;
        int[] y = Nums.bitsOf(other);
        Num n = new PosNum(Arithmetics.divide(x, y));
        if (other.isNeg())
            n = n.inv();
        return n;
    }

}


