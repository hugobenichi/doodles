import java.util.*;

import streams.*;

public class SumOf {

    public static int sum(List<Integer> list) {
        Operator<Integer,Integer> sum = new Operator<Integer,Integer>() {
            public Integer call(Integer s, Integer x) {
                return Integer.valueOf(s + x);
            }
        };
        return Streams.from(list).reduce(sum).call(null);
    }

    public static int average(List<Integer> list) {

        Operator<Integer,Integer> average = new Operator<Integer,Integer>() {
            int total = 0;
            int sum = 0;
            public Integer call(Integer s, Integer x) {
                total++;
                sum = 
                return Integer.valueOf((sum)/total);
            }
        };
        return Streams.from(list).reduce(average).call(null);
    }

    public static void example() {


        List<Integer> numbers = Arrays.asList(0,1,2,3,4,5,6);

        System.out.println(sum(numbers));


        List<String> strings = Arrays.asList("foo", "bar", "bob", "cafebabe");

        for (String s : reverse(strings) ) { System.out.println(s); }


    }

    public static void main(String argv[]) {

        example();

        System.exit(0);

    }
}
