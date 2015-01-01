import java.util.*;


public class QSort {

    private void swap(StringBuffer string, int from, int to) {
        char temp = string.charAt(from);
        string.setCharAt(from, string.charAt(to));
        string.setCharAt(to, temp);
    }

    public static int qsort_pivot(StringBuffer string, int from, int to) {
        int pivot_i = (from+to)/2;
        char pivot = string.charAt(pivot_i);
        swap(string, pivot_i, to);
        pivot_i = to;
        while(from < pivot_i) {
            if (string.charAt(from) < pivot) {
                from++;
            } else {
                swap(string, from, pivot_i-1);
                swap(string, pivot_i-1, pivot);
                pivot_i--;
            }
        }
        int pivot_final = pivot_i;
        while(string.charAt(pivot_i) == pivot) pivot_i++;
        while(pivot_i < to) {
            if ( string.charAt(to) == pivot ) {
                swap(string, pivot_i, to);
                pivot_i++;
            }
            to--;
        }
        return pivot_final;
    }

    public static void qsort(StringBuffer string, int from, int to) {
        if ( from >= to) { System.out.println(); return; }
        if ( from + 1 == to) {
            if (string.charAt(from) > string.charAt(to)) swap(string, from, to);
            return;
        }

        int pivot_final = qsort_pivot(string, from, to);

        qsort(string, from, pivot_final-1);
        char pivot = string.charAt(pivot_final);
        while( string.charAt(pivot_final) == pivot) pivot_final++;
        qsort(string, pivot_final, to);
    }

    public static void main(String argv[]) {
        if (argv.length == 0) System.exit(1);

        StringBuffer string = new StringBuffer(argv[0]);

        System.out.println(string);
        qsort(string, 0, string.length()-1);
        System.out.println(string);

        System.exit(0);
    }

}
