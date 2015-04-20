import java.util.*;

import streams.*;

public class Buffer {

    public static Stream<String> expensive(Iterable<String> list) {

        Function<String,String> print_msg = new Function<String,String>(){
            public String call(String s) {
                System.out.printf("very expensive operation ... ");
                return s;
            }
        };
        return Streams.from(list).map(print_msg);

    }

    public static void example() {

        List<String> strings = Arrays.asList("foo", "bar", "alice", "cafebabe");

        Stream<String> results = expensive(strings);

        System.out.println("\nno buffer:");
        for (String s : results) { System.out.println(s); }
        for (String s : results) { System.out.println(s); }

        Stream<String> buffered_results = Streams.buffer(results);

        System.out.println("\nwith buffer:");
        for (String s : buffered_results) { System.out.println(s); }
        for (String s : buffered_results) { System.out.println(s); }

    }

    public static void main(String argv[]) {
        example();
        System.exit(0);
    }

}
