import java.util.*;

import streams.*;

public class StreamsExample {

    private StreamsExample() {}

    public static void main(String argv[]) {

        String[] string_array = {"foo", "bar", "alice", "bob", "cafebabe"};

        List<String> strings = Arrays.asList(string_array);

        Function<String,Integer> length_of = new Function<String,Integer>(){
            public Integer call(String s) { return Integer.valueOf(s.length()); }
        };

        for (Integer l: Streams.from(strings).map(length_of) ) {
            System.out.println(l);
        }

        System.exit(0);

    }
}
