package streams;

import java.lang.Iterable;
import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;

import streams.Stream;
import streams.AbstractStream;

/**
 * A static helper class for the Stream interface.
 * @author hugo benichi
 * @version 0.1.1
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
     * AbstractStream. This anonymous instance simply stores a reference to the
     * iterable object and implements Iterable#iterator by delegation to the
     * underlying Iterable object.
     * @param <E>       the type of the returned Stream.
     * @param sequence  an instance of Iterable serving as the base for the
     * returned Stream object. The parameter type of this Iterable can be a
     * subtype of E. Returns null if sequence is null.
     * @return          a Stream object.
     * @see Stream
     * @see AbstractStream
     */
    public static <E> Stream<E> from(final Iterable<? extends E> sequence) {
        if ( sequence == null ) return null;  /* guard against null */
        return new AbstractStream<E>() {
            final Iterable<? extends E> underlying_seq = sequence;
            public Iterator<E> iterator() {
                return new Iterator<E>(){
                    Iterator<? extends E> iter = underlying_seq.iterator();
                    public void remove(){ iter.remove(); }
                    public boolean hasNext() { return iter.hasNext(); }
                    public E next() { return iter.next(); }
                };
            }
        };
    }

    /**
     * Takes the first n elements of a Stream and returns them in a list in
     * iteration order.
     * @param <E>    the type of the returned List stream.
     * @param stream the input stream to buffer into a List. Its type parameter
     * can be a subtype of E. Returns null is stream is null.
     * @param n      the number of elements to take from the stream and put into
     * the returned list. If stream has less than n elements or if n is negative
     * then all the elements selected. If n is zero an empty list is returned.
     * @return       a list which contains the first n elements of the stream in
     * iteration order.
     */
    public static <E> List<E> take(Stream<? extends E> stream, int n){
        if (stream == null) return null;
        List<E> output_list = new LinkedList<E>();
        if ( n != 0) {
            Iterator<? extends E> iter = stream.iterator();
            while( n-- != 0 && iter.hasNext()) { output_list.add(iter.next()); }
        }
        return output_list;
    }

    /**
     * Buffers internally a Stream to avoid rerunning expensive operations (not
     * yet implemented).
     * @param <E>    the type of the input stream.
     * @param stream a stream to buffer. Returns null if stream is null.
     * @return       a wrapping stream which will automatically buffer new
     * computed values.
     */
    public static <E> Stream<E> buffer(Stream<E> stream) {
        if (stream == null) return null;
        return null;
    }

    /**
     * Returns the last value of a Stream. Used by AbstractStream in conjunction
     * with Stream#map and Streams#fold_with_map to implement Stream#fold
     * and Stream#reduce.
     * @param <E>    the type of the input stream.
     * @param stream a Stream object. Returns null is stream is null.
     * @return       the last value of this Stream or null if it is empty.
     * @see AbstractStream#fold
     * @see Streams#fold_with_map
     */
    public static <E> E last(Stream<E> stream) {
        if (stream == null) return null;
        E last = null;
        for (E item : stream) { last = item; }
        return last;
    }

    /**
     * Helper function for implementation of AbstractStream#fold and #reduce.
     * Maps a stream with a Function object and returns the last object of this
     * mapped stream. The Function object is actually a closure wrapping a fold
     * operation and returns the updated state of the fold for every items in
     * the input stream. Therefore the result of the fold is found in the last
     * item of the mapped Stream.
     * @param <E>              the type of the stream to fold.
     * @param <F>              the return type of the fold operation.
     * @param stream           the stream to fold. Returns null if stream is
     * null.
     * @param folding_adapter  a closure wrapping a fold operation and presented
     * as a Function instance to adapt to Stream#map interface.
     * @return                 the result of the fold operation.
     * @see AbstractStream#fold
     * @see Streams#last
     */
    public static <E,F> F fold_with_map(
        Stream<E> stream,
        Function<? super E,F> folding_adapter
    ) {
        if (stream == null) return null;
        Stream<F> folding_stream = stream.map(folding_adapter);
        return Streams.last(folding_stream);
    }

}
