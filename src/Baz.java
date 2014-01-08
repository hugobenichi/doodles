import java.util.*;
import java.util.function.*;

class Baz {

  public static void main(String[] args) {

    List<String> l = Arrays.asList("foo","bar","etron","tokyo","paris");

    l.forEach( s -> System.out.println(s) );

    l.stream()
      .map( s -> s.length() )
      .filter( len -> len > 3 )
      .forEach( len -> System.out.println(len) );

    System.out.println("hello java 8");
    System.exit(0);
  }

}
