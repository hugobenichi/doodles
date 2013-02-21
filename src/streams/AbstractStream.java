package streams;

import java.lang.Iterable;
import java.util.Iterator;

import streams.Stream;

/**
 * Abstract class which provides a skeleton implementation of Stream. Clients
 * inheriting from this class only need to implement the iterator method from
 * the Iterable interface.
 * @author hugo benichi
 * @version 0.1.1
 * @see Stream
 */
public abstract class AbstractStream<E> implements Stream<E> {

    /**
     * Clients implement this. Follows the usual Iterator contract.
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
    public <F> Stream<F> map(final Function<? super E, ? extends F> transform) {
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
    public <F> Function<F,F> fold(final Operator<F,E> folder) {

        final Stream<E> input_stream = this;

        return new Function<F,F>() {
            public F call(F input) {
                final F accumulator_init_state = input;
                Function<E,F> folding_adapter = new Function<E,F>() {
                    F accumulator = accumulator_init_state;
                    public F call(E input){
                        accumulator = folder.call(accumulator, input);
                        return accumulator;
                    }
                };
                return Streams.fold_with_map(input_stream, folding_adapter);
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
    public Function<?,E> reduce(final Operator<E,E> reducer) {

        final Stream<E> input_stream = this;

        return new Function<Object,E>() {
            public E call(Object any_input) {
                Function<E,E> reducing_adapter = new Function<E,E>() {
                    boolean is_init = false;
                    E accumulator = null;
                    public E call(E input){
                        if (is_init) {
                            accumulator = reducer.call(accumulator, input);
                        } else {
                            is_init = true;
                            accumulator = input;
                        }
                        return accumulator;
                    }
                };
                return Streams.fold_with_map(input_stream, reducing_adapter);
            }
        };

    }

    private static class State<E> {
        private static final State<Object> NULLSTATE = new State<Object>(null, false);
        public final E value;
        public final boolean valid;
        private State(E value_to_wrap, boolean is_valid) {
            this.value = value_to_wrap;
            this.valid = is_valid;
        }
        public boolean yes() { return this.valid; }
        public boolean no() { return !this.valid; }
        public static <X> State<X> check(X value_to_wrap, boolean is_valid) {
            return new State<X>(value_to_wrap, is_valid);
        }
        public static <X> Function<X,State<X>> adapter(final Predicate<? super X> test) {
            return new Function<X,State<X>>(){
                public State<X> call(X input) {
                    return State.check(input, test.call(input));
                }
            };
        }
        public static <X> State<X> nullState(){ 
            @SuppressWarnings("unchecked")
            State<X> casted_nullstate = (State<X>) State.NULLSTATE;
            return casted_nullstate;
        }
    }

    public Stream<E> select_bis(final Predicate<? super E> check) {
        final Stream<State<E>> yesno_stream = this.map(State.adapter(check));
        return new AbstractStream<E>() {
            public Iterator<E> iterator() {
                return new Iterator<E>(){
                    Iterator<State<E>> iter = yesno_stream.iterator();
                    State<E> next_item = State.nullState();
                    public void remove(){}
                    public boolean hasNext() {
                        find_next_valid();
                        return next_item != State.nullState();
                    }
                    public E next() { 
                        find_next_valid();
                        E next_to_consume = next_item.value;
                        next_item = State.nullState();
                        return next_to_consume;
                    }
                    private void find_next_valid() {
                        State<E> next_candidate = next_item;
                        while(next_candidate.no() && iter.hasNext())
                            next_candidate = iter.next();
                        if (next_candidate.yes())
                            next_item = next_candidate;
                    }
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
    public Stream<E> select(final Predicate<? super E> check) {

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

}
