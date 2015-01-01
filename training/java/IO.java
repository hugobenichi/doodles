import java.nio.file.*;
import java.nio.charset.*;
import java.util.*;
import java.io.*;


class LineIterator implements Iterable<String> {

    private final Path where;

    public LineIterator(Path where) {
        this.where = where;
    }

    public Iterator<String> iterator() {
        return new Iterator<String>() {
            private BufferedReader data; 
            private String next;
            {
                try {
                    data = Files.newBufferedReader(where, StandardCharsets.UTF_8);
                    next = data.readLine();
                } catch (IOException orz) {next = null; data = null;}
            }
            public void remove(){}
            public boolean hasNext(){ return next == null ? false : true; }
            public String next(){
                String current = next;
                try { if (data != null) next = data.readLine(); else next = null;}
                catch (IOException orz) {next = null;}
                return current;
            }
        };
    }

    public static LineIterator from(Path where) {
        return new LineIterator(where);
    }

}

class LineReader {

    public static Iterable<String> from(final Path where) {

        return new Iterable<String>() {

            public Iterator<String> iterator() {
                return new Iterator<String>() {
                    private BufferedReader data; 
                    private String next;
                    {
                        try {
                            data = Files.newBufferedReader(where, StandardCharsets.UTF_8);
                            next = data.readLine();
                        } catch (IOException orz) {next = null; data = null;}
                    }
                    public void remove(){}
                    public boolean hasNext(){ return next == null ? false : true; }
                    public String next(){
                        String current = next;
                        try { if (data != null) next = data.readLine(); else next = null;}
                        catch (IOException orz) {next = null;}
                        return current;
                    }
                };
            }

        };

    }

}

public class IO {

    public static void main(String argv[]){

        if ( argv.length == 0) System.exit(2);

        Path where = Paths.get(argv[0]);
        Charset utf8 = StandardCharsets.UTF_8;
/*
        try {
            
            //for (String l : Files.readAllLines(where, utf8)) { System.out.println(l); }

            BufferedReader data = Files.newBufferedReader(where, utf8);
            String line = data.readLine();
            while(line != null) {
                System.out.println(line);
                line = data.readLine();
            }

        } catch (IOException orz) {
            System.out.println("could not open file " + argv[0]);
        }
*/

        for(String l : LineReader.from(where)) { System.out.println(l); }

        System.exit(0);
    }
}
