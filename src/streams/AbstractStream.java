package streams;

import java.lang.Iterable;
import java.util.Iterator;

import streams.Stream;

/**
 * Abstract class which provides a skeleton implementation of Stream. Clients
 * inheriting from this class only need to implement the Iterable#iterator.
 * @author hugo benichi
 * @version 0.1.2
 * @see Stream
 */
public abstract class AbstractStream<E> implements Stream<E> {

    /**
     * Clients implement this. Follows the usual Iterator contract.
     */
    public abstract Iterator<E> iterator();

    /**
     * Creates a new stream by delegating calls to an underlying iterator taken
     * from the current stream. Function#call is invoked in Iterator#next.
     * @param <F>       the type of the output Stream.
     * @param transform a Function object which specifies how the objects
     * provided by this stream should be transformed. Returns null if transform
     * is null.
     * @return          a new Stream of possibly a different type.
     * @see Stream#map
     * @see Function
     */
    public <F> Stream<F> map(final Function<? super E, ? extends F> transform) {
        if (transform == null) return null;

        final Iterable<E> input_stream = this;

        return new AbstractStream<F>() {
            public Iterator<F> iterator() {
                return new Iterator<F>(){
                    Iterator<E> iter = input_stream.iterator();
                    public void remove(){ iter.remove(); }
                    public boolean hasNext() { return iter.hasNext(); }
                    public F next() { return transform.call(iter.next()); }
                };
            }
        };

    }

    /**
     * Creates an anonymous Function instance which wraps a call to
     * Stream#fold_with_map. In turn, Stream#fold_with_map calls a closure which
     * wraps the Operator folding operation obtained from the caller. This
     * allows to use Stream#map to perform the computation.
     * @param <F>        the type of the output scalar value. May be different
     * from E.
     * @param folder     a Operator object to compute the folded scalar value.
     * Returns null if folder is null.
     * @return            Function object which when its call() method is
     * invoked returns the result of the reduce operation. The input argument to
     * call() is the initial value of the fold operation. The result is not
     * buffered and further invocations of call() will rerun the computation.
     * @see Streams#fold_with_map
     * @see Operator
     */
    public <F> Function<F,F> fold(final Operator<F,E> folder) {
        if (folder == null) return null;

        final Stream<E> input_stream = this;

        return new Function<F,F>() {
            public F call(F input) {

                final F accumulator_init_state = input;

                /* Closure which wraps the folding operation */
                Function<E,F> folding_adapter = new Function<E,F>() {

                    /* keep a reference to the accumulator value */
                    F accumulator = accumulator_init_state;

                    /* deleguate to teh folding operation */
                    public F call(E input){
                        accumulator = folder.call(accumulator, input);
                        return accumulator;
                    }

                };

                /* final call to Streams#fold_with_map to get the last value */
                return Streams.fold_with_map(input_stream, folding_adapter);

            }
        };

    }

    /**
     * Creates an anonymous Function instance which wraps a call to
     * Stream#fold_with_map. In turn, Stream#fold_with_map calls a closure which
     * wraps the Operator reducing operation obtained from the caller. This
     * allows to use Stream#map to perform the computation.
     * @param reducer    a Operator object to compute the reduced scalar value.
     * Returns null if folder is null.
     * @return           a Function object which when its call() method is invoked
     * returns the result of the reduce operation. The Function object takes any
     * value as input and can be called with null. The result is not buffered
     * and further invocations of call() will rerun the computation.
     * @see Streams#fold_with_map
     * @see Operator
     */
    public Function<?,E> reduce(final Operator<E,E> reducer) {
        if (reducer == null) return null;

        final Stream<E> input_stream = this;

        return new Function<Object,E>() {
            public E call(Object any_input) {

                Function<E,E> reducing_adapter = new Function<E,E>() {

                    boolean is_init = false;    /* tells if 1st call or not */
                    E accumulator = null;       /* ref to the reduced value */

                    /* any input will do`, necessary for Function interface*/
                    public E call(E input){
                        if (is_init) {
                            accumulator = reducer.call(accumulator, input);
                        } else {
                            is_init = true;
                            accumulator = input; /*1st call: init accumulator*/
                        }
                        return accumulator;
                    }

                };

                /* final call to Streams#fold_with_map to get the last value */
                return Streams.fold_with_map(input_stream, reducing_adapter);

            }
        };

    }

    /**
     * TODO: DOCME!
     * @param check a Predicate object which specifies which test to run.
     * @return      a new Stream object of the same type.
     * @see Predicate
     */
    public Stream<E> select(final Predicate<? super E> check) {

        final Iterable<E> input_stream = this;
        return new AbstractStream<E>() {

            @SuppressWarnings("unchecked")
            private final E not_valid_token = (E) new Object();

            public Iterator<E> iterator() {

                return new Iterator<E>(){

                    Iterator<E> iter = input_stream.iterator();
                    E next_item = not_valid_token;

                    private void find_next_valid() {
                        if (next_item != not_valid_token) return;
                        while( iter.hasNext() ) {
                            E next_candidate = iter.next();
                            if ( check.call(next_candidate) ) {
                                next_item = next_candidate;
                                break;
                            }
                        }
                    }

                    /* don't deleguate: it could remove the wrong object*/
                    public void remove(){}

                    public boolean hasNext() {
                        find_next_valid();
                        return next_item != not_valid_token;
                    }

                    public E next() {
                        find_next_valid();
                        if (next_item == not_valid_token) return null;
                        E next_to_consume = next_item;
                        next_item = not_valid_token;
                        return next_to_consume;
                    }

                };
            }

        };

    }

}
