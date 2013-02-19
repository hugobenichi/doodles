package streams;

import java.lang.Iterable;
import java.util.Iterator;

import streams.Stream;

/**
 * Abstract class which provides a skeleton implementation of Stream. Clients
 * inheriting from this class only need to provide the iterator method coming
 * from the Iterable interface.
 * @author hugo benichi
 * @version 0.1.0
 * @see Stream
 */
public abstract class AbstractStream<E> implements Stream<E> {

    /**
     * Clients implement this.
     */
    public abstract Iterator<E> iterator();

    /**
     * TODO: DOCME!
     * @param <F>       the type of the output Stream.
     * @param transform a Function object which specifies how the objects
     * provided by this stream should be transformed.
     * @return          a new Stream of possibly a different type.
     * @see Function
     */
    public <F> Stream<F> map(Function<? super E, ? extends F> transform) {
        final Iterable<E> input_stream = this;
        return new AbstractStream<F>() {
            public Iterator<F> iterator() {
                return new Iterator<F>(){
                    Iterator<E> iter = input_stream.iterator();
                    public void remove(){}
                    public boolean hasNext() { return iter.hasNext(); }
                    public F next() { return transform.call(iter.next()); }
                };
            }
        };
    }

    /**
     * TODO: DOCME!
     * @param check a Predicate object which specifies which test to run.
     * @return      a new Stream object of the same type.
     * @see Predicate
     */
    public Stream<E> select(Predicate<? super E> check) {
        return this;
    }

    // TODO: shouldn t first param of Operator be ? extend E ?
    /**
     * TODO: DOCME!
     * @param reducer a Operator object to compute the reduced scalar value.
     * @return        the final state of the reduce operation.
     * @see Operator
     */
    public E reduce(Operator<? extends E, ? super E> reducer) {
        /*
            get init_state from iterator
            call fold to leverage
        */
        return null;
    }

    // TODO: check food type of init_state
    /**
     * TODO: DOCME!
     * @param <F>        the type of the output scalar value. May be different
     * from E.
     * @param folder     a Operator object to compute the reduced scalar value.
     * @param init_state the initial state of the returned scalar value.
     * @return           the final state of the fold operation.
     * @see Operator
     */
    public <F> F fold(Operator<? extends F, ? super E> folder, F init_state) {
        return null;
        /*
            prepare map operator which apply folder and return init_state
            define stream with this map operator
            return last state with last
        */
    }

}
