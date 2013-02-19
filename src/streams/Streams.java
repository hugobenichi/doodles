package streams;

import java.lang.Iterable;
import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;

import streams.Stream;
import streams.AbstractStream;

/**
 * A static helper class for the Stream interface.
 * TODO: finish comments here
 * @author hugo benichi
 * @version 0.1.0
 * @see Stream
 */
public final class Streams {

    /**
     * Not instantiable
     */
    private Streams() {}

    /**
     * Factory method which takes an Iterable object and wraps it inside a
     * Stream object produced on the fly as an anonymous instance of
     * AbstractStream. This anonymous instance simply store a reference to the
     * iterable and implements Iterable#iterator by delegation to the underlying
     * Iterable object.
     * @param <E>       the type of the underlying Iterable instance
     * @param sequence  a instance of Iterable serving as the base for the
     * constructed Stream
     * @return          a Stream of the same type of the Iterable
     * @see Stream
     * @see AbstractStream
     */
    public static <E> Stream<E> from(final Iterable<? extends E> sequence) {
        return new AbstractStream<E>() {
            final Iterable<? extends E> underlying_seq = sequence;
            public Iterator<E> iterator() {
                return new Iterator<E>(){
                    Iterator<? extends E> iter = underlying_seq.iterator();
                    public void remove(){}
                    public boolean hasNext() { return iter.hasNext(); }
                    public E next() { return iter.next(); }
                };
            }
        };
    }

    // TODO: return type ? Iterable<E> ?
    /**
     * TODO: DOCME!
     * @param <E>    the type of the input stream
     * @param stream a\n input stream to buffer into a List
     * @param n      the number of element to take from stream. If stream has
     * less than n element, then all the elements of stream are put into the
     * output list. If n is zero or negative, an empty list is returned.
     * @return       a list which contains the first n elements of the stream in
     * iteration order.
     */
    public static <E> List<E> take(Stream<E> stream, int n){
        List<E> output_list = new LinkedList<E>();
        if ( n != 0) {
            Iterator<E> iter = stream.iterator();
            while( n-- != 0 && iter.hasNext()) { output_list.add(iter.next()); }
        }
        return output_list;
    }

    // change input type to Iterarable<E> ?
    /**
     * TODO: DOCME!
     * @param <E>    the type of the input stream
     * @param stream a stream to buffer.
     * @return       a wrapping stream which will automatically buffer new
     * computed values.
     */
    public static <E> Stream<E> buffer(Stream<E> stream) {
        return null;
    }

    /**
     * Return the last value of a Stream. Used by AbstractStream in conjunction
     * with Stream#map to implement Stream#fold.
     * @param <E>    the type of the input stream
     * @param stream a stream
     * @return       the last value of this Stream or null if it's the empy
     * Stream
     * @see AbstractStream#fold
     */
    public static <E> E last(Stream<E> stream) {
        E last = null;
        for (E item : stream) { last = item; }
        return last;
    }

    public static void foo() { System.out.println("foo"); }

}
