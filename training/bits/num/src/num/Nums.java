package num;

import java.util.Arrays;

public class Nums {

    public static Num toNum(int x) {
        int[] bits = new int[1];
        bits[0] = x & 0x7fff_ffff;
        Num n = new PosNum(bits);
        if ( x < 0) n = n.inv(); 
        return n;
    }

    public static int[] bitsOf(Num number) {
        if (AbstractBitsNum.class.isInstance(number))
            return ((AbstractBitsNum) number).bits();
        return bitsOf(number.bitstring());
    }

    public static int[] bitsOf(String bitstring) {
    
        char[] bits_char = bitstring.toCharArray();

        int bits_len = 1 + (bits_char.length - 1)/ 31;
        int[] bits = new int[bits_len];
        Arrays.fill(bits, 0);
        
        int mask = 1;
        int bits_index = 0;
        for (int i = bits_char.length - 1; i >= 0; --i) {
            if ( bits_char[i] == '1')
                bits[bits_index] += mask; 
            mask = mask << 1;
            if (mask == 0x4000_0000) {
                mask = 1;
                bits_index++;
            }
        }
 
        return bits;
    }

    public static String bitstring(int[] bits) {
        StringBuilder string = new StringBuilder();
        for (int i = 0; i != bits.length; ++i) {
            string.insert(0, Integer.toBinaryString(bits[i]));
        }
        return string.toString();
    }

    /* wrong !, need to take into account carry bit between segments */
    public static String decstring(int[] bits) {
        StringBuilder string = new StringBuilder();
        for (int i = 0; i != bits.length; ++i) {
            string.insert(0, Integer.toString(bits[i]));
        }
        return string.toString();
    }

}
