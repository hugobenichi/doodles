import java.util.*;

import streams.*;

public class Select {

    public static Stream<Integer> less_than(Iterable<Integer> list, final int x) {
        Predicate<Integer> is_less = new Predicate<Integer>() {
            public boolean call(Integer y) { return y.intValue() < x; }
        };
        return Streams.from(list).select(is_less);
    }

    public static Stream<Integer> even(Iterable<Integer> list) {
        Predicate<Integer> is_even = new Predicate<Integer>() {
            public boolean call(Integer y) { return (y.intValue() & 1) == 0; }
        };
        return Streams.from(list).select(is_even);
    }

    public static <E> Stream<E> no_null(Iterable<E> list) {
        Predicate<E> is_not_null = new Predicate<E>() {
            public boolean call(E y) { return y != null; }
        };
        return Streams.from(list).select(is_not_null);
    }

    public static void example() {

        List<Integer> numbers = Arrays.asList(0,1,2,3,4,5,6);
        
        System.out.print("from array: ");
        System.out.println( JoinArray.join(even(numbers)," ") );

        System.out.print("  less than 4: ");
        System.out.println( JoinArray.join(less_than(numbers, 4)," ")  );

        System.out.print("  even numbers: ");
        //System.out.println( JoinArray.join(even(numbers), " ")  );

        List<String> strings = Arrays.asList("foo",null,"bar","cafebabe",null, null);

        System.out.print("from array: ");
        System.out.println( JoinArray.join(strings," ") );

        System.out.print("  non nulls: ");
        System.out.println( JoinArray.join(no_null(strings), " ")  );


    }

    public static void main(String argv[]) {
        example();
        System.exit(0);
    }

}
