import java.util.*;

public class Parent {

    public static void print_all_perm(char[] buffer, int left_min_right, int index) {
/*
        if (index == buffer.length) {
            System.out.println(buffer);
            return;
        }
*/
        if (left_min_right == buffer.length - index) {
            complete_and_print(buffer, index);
            return;
        }

        buffer[index] = '(';
        print_all_perm(buffer, left_min_right+1, index+1);

        if (left_min_right == 0)
            return;

        buffer[index] = ')';
        print_all_perm(buffer, left_min_right-1, index+1);

    }

    public static void complete_and_print(char buffer[], int index) {
        while (index != buffer.length) {
            buffer[index++] = ')';
        }
        System.out.println(buffer);
    }


    public static void print_all(int n_pairs) {
        char[] buffer = new char[2*n_pairs];
        print_all_perm(buffer, 0, 0);
    }

    public static void main(String argv[]) {
        int n_pairs = 2;
        if(argv.length > 0) {
            try { n_pairs = Integer.parseInt(argv[0]); }
            catch (NumberFormatException orz) {}
        }
        print_all(n_pairs);
        System.exit(0);
    }

}
