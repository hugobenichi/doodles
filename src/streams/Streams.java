package streams;

import java.lang.Iterable;

import streams.Stream;
import streams.AbstractStream;

public class Streams {

    private Streams() {} /* no instance please */

    public static <E> Stream<E> from(Iterable<? extends E> sequence) {
        return null;
    }

    public static void foo() { System.out.println("foo"); }

}
