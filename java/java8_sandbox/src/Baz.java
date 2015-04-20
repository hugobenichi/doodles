import java.util.*;
import java.util.function.*;
import java.util.stream.*;

class Baz {

  public static Integer lenOf(String s) { return s.length(); }

  public static Function<String,Integer> lenOfFunc() {
    return Baz::lenOf;
  }

  public static void main(String[] args) {

    List<String> sl = Arrays.asList("foo","bar","baz","tokyo","paris");

    sl.forEach( s -> System.out.println(s) );

    int offset = 0;

    Stream<Integer> is =
      sl.stream()
        //.map( s ->  offset + s.length());
        //.map( s -> { System.out.println(s); return offset + s.length(); });
        .map(lenOfFunc());

    System.out.println("ergoherg");

     is.filter( len -> len > 3 )
       .forEach( len -> System.out.println(len) );

  }

}
