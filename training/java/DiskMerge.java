import java.util.*;
import java.nio.file.*;
import java.nio.charset.*;
import java.io.*;

public class DiskMerge {

    public static Iterator<Integer> getreader(String path) {
        return new Iterator<Integer>() {
            BufferedReader reader;
            String next;
            {
                next = null;
                try {
                    Path where = Paths.get(path);
                    Charset utf8 = StandardCharsets.UTF_8;
                    reader = Files.newBufferedReader(where, utf8);
                    next = reader.readLine();
                } catch (IOException orz) {}
            }
            public void remove() {}
            public boolean hasNext() { return next == null ? false : true; }
            public Integer next() {
                if (next == null) return -1;
                int number = -1; 
                try { number = Integer.parseInt(next); next = reader.readLine(); }
                catch (NumberFormatException orz) { }
                catch (IOException orz) { next = null;}
                return number;
            }
        };
    }
    
    public static Iterator<Integer> minof(final Iterator<Integer> a, final Iterator<Integer> b) {
        return new Iterator<Integer>() {
            Iterator<Integer> it_a = a, it_b = b; 
            Integer next_a = null, next_b = null;
            { 
                if(it_a.hasNext()) next_a = it_a.next();
                if(it_b.hasNext()) next_b = it_b.next();
            }
            public void remove() {}
            public boolean hasNext() { return ( (next_a != null) || (next_b != null) ); }
            public Integer next() { 
                Integer min = next_a;
                if ( next_a == null || (next_b != null && next_b < next_a) ) {
                    min = next_b;
                    next_b = it_b.hasNext()? it_b.next() : null;
                }
                else
                    next_a = it_a.hasNext()? it_a.next() : null;
                return min;
            }
        };
    }

    public static void main(String argv[]) {
        if (argv.length < 2) System.exit(0);
    
        Iterator<Integer> file1 = getreader(argv[0]), file2 = getreader(argv[1]);
    
        Iterator<Integer> min = minof(file1, file2);
        
        while(min.hasNext()) { System.out.println(min.next()); }
    
        System.exit(0);
    }

}
