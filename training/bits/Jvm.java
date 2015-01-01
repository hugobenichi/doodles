import java.io.*;

public class Jvm {

    public static PrintStream cout = System.out;

    public static void bin(long x){
        String bits = Long.toBinaryString(x);
        int padding = 64 - bits.length();
        char[] zeros = new char[padding];
        for(int i = 0; i != padding; ++i) zeros[i] = '0';

        StringBuilder string = new StringBuilder();
        string.append(zeros);
        string.append(bits);
        string.insert(32, '_');
        cout.println(string.toString());
    }

    public static void bin(int x){ bin((long)x); }

    public static void withint(){
        int x = Integer.MAX_VALUE;
        cout.println(x);
        bin(x);

        int y = 12345;
        int z = x + y;
        bin(y);
        bin(z);
        bin(z & 0x7fffffff);
    }

    public static void withlong(){
        cout.println(Long.MAX_VALUE);
        bin(Long.MAX_VALUE);

        long mask = 0x7fff_ffff; //_ffff_ffff;
        long x = Integer.MAX_VALUE;
        long y = 12345 << 10;
        long z = x * y;
        bin(x);
        bin(y);

        cout.println();
        bin(z);
        bin(z & Integer.MAX_VALUE);
        bin( (z >> 31) & Integer.MAX_VALUE);
    }

    public static void main(String argv[]) {

        cout.println(Integer.MAX_VALUE >> 31);
        cout.println(Integer.MIN_VALUE >> 31);

        cout.println(Integer.MAX_VALUE >>> 31);
        cout.println(Integer.MIN_VALUE >>> 31);

        cout.println(Integer.MAX_VALUE >> 32);
        cout.println(Integer.MIN_VALUE >> 32);

        cout.println(Integer.MAX_VALUE >>> 32);
        cout.println(Integer.MIN_VALUE >>> 32);

/*
        cout.println("\nwith int");
        withint();

        cout.println("\nwith long");
        withlong();
*/
        System.exit(0);
    }

}
