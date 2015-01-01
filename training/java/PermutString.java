import java.util.*;


public class PermutString {

    public static void print_permut_unique(String string) {
        char sorted_string[] = string.toCharArray();
        Arrays.sort(sorted_string);
        rec_permut_unique(sorted_string, 0);
    }

    public static void rec_permut_unique(char string[], int tail_index) {
        if (tail_index + 1 == string.length ) {
            System.out.println(string);
            return;
        }
        rec_permut_unique(string, tail_index + 1);
        for (int i = tail_index + 1; i < string.length; ++i) {
            if( string[tail_index] == string[i]) continue;
            if( string[i-1] == string[i]) continue;
            swap(string, tail_index, i);
            rec_permut_unique(string, tail_index + 1);
            swap(string, tail_index, i);
        }
    }

    public static void print_permut(String string) {
        rec_permut(string.toCharArray(), 0);
    }

    public static void rec_permut(char string[], int tail_index) {
        if (tail_index + 1 == string.length ) {
            System.out.println(string);
            return;
        }
        for (int i = tail_index; i < string.length; ++i) {
            swap(string, tail_index, i);
            rec_permut_unique(string, tail_index + 1);
            swap(string, tail_index, i);
        }
    }

    public static void swap(char string[], int i, int j) {
        char x = string[i];
        string[i] = string[j];
        string[j] = x;
    }

    public static void main(String argv[]) {
        if (argv.length == 0) System.exit(1);

        print_permut_unique(argv[0]);
//        print_permut(argv[0]);

        System.exit(0);
    }

}
