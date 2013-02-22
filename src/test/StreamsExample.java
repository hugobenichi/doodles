import java.util.*;

import streams.*;

public class StreamsExample {

    private StreamsExample() {}

    public static void main(String argv[]) {

        String[] string_array = {"foo", "bar", "alice", "bob", "cafebabe"};

        Stream<String> strings = Streams.from(Arrays.asList(string_array));


        Function<String,Integer> length_of = new Function<String,Integer>(){
            public Integer call(String s) { return Integer.valueOf(s.length()); }
        };

        for (Integer l: strings.map(length_of) ) {
            System.out.println(l);
        }

        Operator<Integer,Integer> sum = new Operator<Integer,Integer>() {
            public Integer call(Integer s, Integer x) {
                return Integer.valueOf(s + x);
            }
        };

        System.out.println(strings.map(length_of).reduce(sum).call(null));

        System.exit(0);

    }
}
