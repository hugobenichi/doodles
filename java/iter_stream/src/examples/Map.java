import java.util.*;

import streams.*;

public class Map {

    public static Stream<Integer> length(Iterable<String> list) {
        Function<String,Integer> length_of = new Function<String,Integer>(){
            public Integer call(String s) { return Integer.valueOf(s.length());}
        };
        return Streams.from(list).map(length_of);
    }

    public static void example() {

        List<String> strings = Arrays.asList("foo", "bar", "alice", "cafebabe");

        for (Integer x : length(strings) ) { System.out.println(x); }

    }

    public static void main(String argv[]) {
        example();
        System.exit(0);
    }

}
