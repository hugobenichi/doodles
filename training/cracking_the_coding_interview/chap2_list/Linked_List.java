import java.util.*;
import java.io.*;

public class Linked_List {

    public static final PrintStream stdout = System.out;

    /* 2.1 p.77 */
    public static <E> List<E> rem_dup(List<E> in) {

        if ( in == null ) return null;

        Set<E> visited = new HashSet<E>();

        ListIterator<E> seq = in.listIterator();
        while( seq.hasNext()) {
            boolean is_new = visited.add( seq.next());
            if ( !is_new ) seq.remove();
        }
        return in;
/*
        List<E> out = new LinkedList<E>();
        for ( E x: in) {
            boolean is_new = visited.add(x);
            if ( is_new ) out.add(x);
        }
        return out;
*/
    }

/*

      ^
 abccba | abc
   ^

        ^
 abcdcba | abc
    ^

*/

    public static <E> boolean is_palyndrome2(List<E> l) {
        if (l == null || l.isEmpty() ) return false;

        LinkedList<E> stack = new LinkedList<E>();
        Iterator<E> seq_fast = l.iterator();
        Iterator<E> seq_slow = l.iterator();

        while( seq_fast.hasNext() ) {
            seq_fast.next();
            E next = seq_slow.next();
            if ( seq_fast.hasNext()) {
                stack.push(next);
                seq_fast.next();
            }
        }

        while(seq_slow.hasNext())
            if ( seq_slow.next() != stack.pop())
                return false;

        return true;
    }

    public static <E> boolean is_palyndrome(List<E> l) {
        if (l == null || l.isEmpty() ) return false;
        return is_pal_rec(null, l.iterator(), l.iterator());
    }

    public static <E> boolean is_pal_rec( E top, Iterator<E> slow, Iterator<E> fast ) {

        if ( fast.hasNext() ) {
            fast.next();
            E next = slow.next();
            boolean is_pal = true;
            if ( fast.hasNext() ) {
                fast.next();
                is_pal = is_pal_rec(next, slow, fast);
            } else {
                is_pal = is_pal_rec(top, slow, fast); // problem in this branch
            }
            if ( ! slow.hasNext()) return is_pal;

            return is_pal && ( top == slow.next());
        }

        return slow.hasNext() ? (top == slow.next()) : true;

    }

    public static void main(String argv[]) {

        List<String> foobar = new LinkedList<String>();
        foobar.add("f");
        foobar.add("o");
        foobar.add("o");
        foobar.add("b");
        foobar.add("a");
        foobar.add("r");
        stdout.println( "should be false: " + is_palyndrome(foobar) );


        List<String> abc = new LinkedList<String>();
        abc.add("a");
        abc.add("b");
        abc.add("c");
        abc.add("c");
        abc.add("b");
        abc.add("a");
        stdout.println( "should be true: " + is_palyndrome(abc) );

        List<String> radar = new LinkedList<String>();
        radar.add("r");
        radar.add("a");
        radar.add("d");
        radar.add("a");
        radar.add("r");
        stdout.println( "should be true: " + is_palyndrome(radar) );

        List<String> empty = new LinkedList<String>();
        stdout.println( "should be false: " + is_palyndrome(empty) );

        List<String> one = new LinkedList<String>();
        one.add("a");
        stdout.println( "should be true: " + is_palyndrome(one) );

        List<String> two_r = new LinkedList<String>();
        two_r.add("a"); two_r.add("a");
        stdout.println( "should be true: " + is_palyndrome(two_r) );

        List<String> two_f = new LinkedList<String>();
        two_f.add("a"); two_f.add("b");
        stdout.println( "should be false: " + is_palyndrome(two_f) );

        System.exit(0);
    }

}
