import java.nio.file.*;
import java.nio.charset.*;
import java.util.*;
import java.io.*;

public class LineReader {

    public static Iterable<String> from(String path) {

        final Path where = Paths.get(path);

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
