import java.util.*;

public class Sorting {


    public static void merge_buf(int[] A, int lA, int[] B, int lB) {
        int i_merge = lA-- + lB-- - 1;
        while ( lA >= 0 || lB >= 0 ) {
            if ( lB < 0 || ( lA >= 0 && A[lA] > B[lB] ) ) {
                A[i_merge--] = A[lA--];
            } else {
                A[i_merge--] = B[lB--];
            }
        }
    }

/*
    -> (0 1 2 3 4 5)
        (0+5)/2 = 2
        check x == A[2]
        -> (0, 1)
            (0 + 1)/2 = 0
            check x == A[0]
                -> (0, -1)
                    return -1
                -> (1, 1)
                    check x == A[1]
        -> (3, 4, 5)
            (3+5)/2 = 4
            check x == A[4]
                -> (3, 3)
                    check x == A[3]
                -> (5,5)
                    check x == A[5]
*/

    public static int binsearch(int x, int[] A, int from, int to) {
        if (from > to || from == to)  return A[from] == x ? -1 : from;

        int i_pivot = (from + to) / 2;
        int pivot = A[i_pivot];

        if ( x == pivot)
            return i_pivot;

        if ( x < pivot)
            return binsearch(x, A, from, pivot-1);
        else
            return binsearch(x, A, pivot+1, to);

    }



    public static void main(String argv[]) {

        int lA = 12;int[] A = {2,3,4,4,6,7,8,10,11,13,20,25,0,0,0,0,0,0};
        int lB = 6; int[] B = {1,3,5,9,14,19};

        merge_buf(A,lA,B,lB);

        StringBuffer string = new StringBuffer();
        for (int i = 0; i != lA+lB; ++i) {
            string.append(A[i]);
            string.append(" ");
        }
        System.out.println(string.toString());

        System.exit(0);

    }

}
