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

        final Iterable<E> input_stream = this;
        return new AbstractStream<E>() {

            public Iterator<E> iterator() {
                return new Iterator<E>(){

                    Iterator<E> iter = input_stream.iterator();
                    E next_item = null;
                    boolean is_next_valid = false;

                    private boolean find_next_valid() {
                        while( is_next_valid == false && iter.hasNext() ) {
                            E next_candidate = iter.next();
                            if ( check.call(next_candidate) ) {
                                is_next_valid = true;
                                next_item = next_candidate;
                            }
                        }
                        return is_next_valid;
                    }

                    public void remove(){}
                    public boolean hasNext() {
                        if (is_next_valid == false)
                            is_next_valid = find_next_valid();
                        return is_next_valid;
                    }

                    public E next() { 
                        if (is_next_valid == false)
                            is_next_valid = find_next_valid();
                        return is_next_valid ? next_item : null;
                    }

                };
            }

        };

    }

    // TODO: shouldn t first param of Operator be ? extend E ?
    /**
     * TODO: DOCME!
     * @param reducer a Operator object to compute the reduced scalar value.
     * @return        the final state of the reduce operation.
     * @see Operator
     */
    public E reduce(Operator<E, ? super E> reducer) {

        Function<E,E> reducing_adapter = new Function<E,E>() {
            boolean is_init = false;
            E accumulator_state = null;
            public E call(E input){
                if (is_init) {
                    accumulator_state = reducer.call(accumulator_state, input);
                } else {
                    is_init = true;
                    accumulator_state = input;
                }
                return accumulator_state;
            }
        };

        return fold_from_map(reducing_adapter);
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
    public <F> F fold(Operator<F, ? super E> folder, F init_state) {

        Function<E,F> folding_adapter = new Function<E,F>() {
            F accumulator_state = init_state;
            public F call(E input){
                accumulator_state = folder.call(accumulator_state, input);
                return accumulator_state;
            }
        };

        return fold_from_map(folding_adapter);
    }

    // helper function for fold and reduce
    // TODO: DOCME!
    private <F> F fold_from_map(Function<E,F> folding_adapter) {
        Stream<F> folding_stream = this.map(folding_adapter);
        return Streams.last(folding_stream);
    }

}
