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
    public static <E> Stream<E> from(final Iterable<E> sequence) {
                           //TODO: why not Iterable< error: reference not found? extends E>
        return new AbstractStream<E>() {
            final Iterable<E> underlying_seq = sequence;
            public Iterator<E> iterator() { return underlying_seq.iterator(); }
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
        return new LinkedList<E>();
    }

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
        return this.iterator().next();
    }
    
    public static void foo() { System.out.println("foo"); }

}
