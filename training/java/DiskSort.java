import java.util.*;
import java.nio.file.*;
import java.nio.charset.*;
import java.io.*;

public class DiskSort {

    public static ArrayList<Integer> parse(String path) {
        if (path == null) return null;
        try {
            ArrayList<Integer> numbers = new ArrayList<Integer>();
            Path where = Paths.get(path);
            Charset utf8 = StandardCharsets.UTF_8;
            BufferedReader reader = Files.newBufferedReader(where, utf8);
            String line = reader.readLine();
            while( line != null) {
                numbers.add( Integer.decode( line ) );
                line = reader.readLine();
            }
            return numbers;
        } catch (IOException orz) {return null;}
          catch (NumberFormatException orz) {return null;}
    }

/*
    public static ArrayList<Integer> merge(ArrayList<Integer> num1, ArrayList<Integer> num2) {
        Iterator<Integer> min = minof(num1.iterator(), num2.iterator());
        ArrayList<Integer> merged = new ArrayList<Integer>();
        while (min.hasNext()) { merged.add( min.next()); }
        return merged;
    }
*/
    public static void mergesort(ArrayList<Integer> numbers, int from, int to) {
        System.out.printf("from %d to %d", from, to);
        if (to <= from ) { System.out.println(); return;}
        if (to == from + 1) { System.out.printf(" "); sort2(numbers, from, to); return;}

        int pivot = (from + to) / 2;
        System.out.printf(" pivot %d\n", pivot);

        mergesort(numbers, from, pivot);
        mergesort(numbers, pivot+1, to);
        merge(numbers, from, pivot, pivot+1, to);
    }

    public static void sort2(ArrayList<Integer> numbers, int from, int to) {
System.out.printf("[%d]=%d [%d]=%d", from, numbers.get(from), to, numbers.get(to));
        Integer x = numbers.get(from), y = numbers.get(to);
        Integer z = x < y ? x : y;
        if ( z == y) {
            numbers.set(to, x);
            numbers.set(from, y);
        }
System.out.printf(" -> [%d]=%d [%d]=%d\n", from, numbers.get(from), to, numbers.get(to));
    }

    public static void merge(ArrayList<Integer> numbers, int from1, int to1, int from2, int to2) {
        int origin = from1;
        System.out.printf("  merging (%d, %d) and (%d, %d)\n", from1, to1, from2, to2);
        ArrayList<Integer> merged = new ArrayList<Integer>();
        while( from1 <= to1 || from2 <= to2) {
            Integer x =  from1 <= to1 ? numbers.get(from1) : null;
            Integer y =  from2 <= to2 ? numbers.get(from2) : null;
            System.out.printf("    which of (%d, %d)", x, y);
            Integer z = x;
            do {
                if ( y == null ) { from1++; break;} 
                if ( x == null || ( x > y) ) { z = y; from2++; break;}
                from1++;
            } while(false);
            System.out.printf(" -> %d\n", z);
            merged.add(z);
        }

        for (int i = 0; i < merged.size(); i++) { numbers.set(origin + i, merged.get(i)); }
    }

    public static void main(String argv[]) {

        if( argv.length == 0 ) System.exit(1);

        ArrayList<Integer> numbers = parse(argv[0]);

        if (numbers == null) System.exit(1);

        mergesort(numbers, 0, numbers.size()-1);

        for (int i : numbers) { System.out.println(i); }

        System.exit(0);
    }

}
