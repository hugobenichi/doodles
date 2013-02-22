import java.util.*;

import streams.*;

public class ReverseList {

    public static <E> List<E> reverse(Iterable<E> list) {
        Operator<List<E>,E> prepend = new Operator<List<E>,E>() {
            public List<E> call(List<E> l, E x) {
                l.add(0, x);
                return l;
            }
        };
        return Streams.from(list).fold(prepend).call(new LinkedList<E>());
    }

    public static void example() {

        List<String> strings = Arrays.asList("foo", "bar", "bob", "cafebabe");

        for (String s : reverse(strings) ) { System.out.println(s); }


        List<Integer> numbers = Arrays.asList(0,1,2,3,4,5,6);

        for (Integer x : reverse(numbers) ) { System.out.println(x); }

    }

    public static void main(String argv[]) {
        example();
        System.exit(0);
    }

}
