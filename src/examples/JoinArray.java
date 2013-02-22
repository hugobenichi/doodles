import java.util.*;

import streams.*;

public class JoinArray {

    public static <E> Stream<String> to_string(Iterable<E> list) {
        Function<E,String> to_str = new Function<E,String>() {
            public String call(E input) { return input.toString(); }
        };
        return Streams.from(list).map(to_str);
    }

    public static <E> String join(Iterable<E> list, final String link) {

        Operator<StringBuilder,String> append = new Operator<StringBuilder,String>() {
            public StringBuilder call(StringBuilder buffer, String s) {
                buffer.append(s);
                buffer.append(link);
                return buffer;
            }
        };

        StringBuilder buffer = new StringBuilder();

        to_string(list).fold(append).call(buffer);

        return buffer.substring(0, buffer.length() - link.length() );
    }

    public static void example() {

        List<String> strings = Arrays.asList("foo", "bar", "bob", "cafebabe");
        System.out.println( join(strings, ",") );


        List<Integer> numbers = Arrays.asList(0,1,2,3,4,5,6);
        System.out.println(join(numbers, " + "));

    }

    public static void main(String argv[]) {
        example();
        System.exit(0);
    }

}
