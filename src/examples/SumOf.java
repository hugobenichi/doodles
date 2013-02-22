import java.util.*;

import streams.*;

public class SumOf {

    public static int sum(Iterable<Integer> list) {
        Operator<Integer,Integer> sum = new Operator<Integer,Integer>() {
            public Integer call(Integer s, Integer x) {
                return Integer.valueOf(s + x);
            }
        };
        return Streams.from(list).reduce(sum).call(null);
    }

    public static void example() {

        List<Integer> numbers = Arrays.asList(0,1,2,3,4,5,6);

        System.out.println(sum(numbers));

    }

    public static void main(String argv[]) {
        example();
        System.exit(0);
    }

}
