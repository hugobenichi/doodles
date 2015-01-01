package num;

import java.util.*;

public class Arithmetics {

    public static int[] plus(int[] x, int[] y) {
        int[] max = x;
        int[] min = y;

        if (y.length > x.length) { max = y; min = x; }

        int[] z = Arrays.copyOf(max, max.length);

        int carry = 0;
        int i = 0;
        for(; i != min.length; ++i){
            z[i] += min[i] + carry;
            carry = z[i] >>> 31;             // 1 if first bit 1 or 0 otherwise
            //carry = 1 & -( z[i] & Integer.MIN_VALUE );
            z[i] = z[i] & Integer.MAX_VALUE; // remove sign bit 
        }

        if (carry == 1) {
            if (i == max.length) {
                z = Arrays.copyOf(z, z.length + 1);
                z[i] = 0;
            }
            z[i] += 1;
        }

        return z;
    }

    public static int[] minus(int[] x, int[] y) {
        return x;
    }

    public static int[] multiply(int[] x, int[] y) {
        return x;
    }

    public static int[] divide(int[] x, int[] y) {
        return x;
    }

    public static int compare(int[] x, int[] y) {
        if (x.length != y.length) return x.length > y.length ? +1 : -1;

        for (int i = x.length - 1; i >= 0; -- i) {
            if ( x[i] != y[i] ) return x[i] > y[i] ? +1 : -1;
        }

        return 0;
    }

}
