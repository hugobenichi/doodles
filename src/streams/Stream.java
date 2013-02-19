package streams;

import java.lang.Iterable;
import java.util.List;

import streams.Function;
import streams.Operator;
import streams.Predicate;

/**
 * The main interface of this package. It defines a wrapper for the
 * Iterable interface and add a number of useful functions to apply and chain
 * operations on the underlying iterator, to eventually produce either a scalar
 * value with Stream#reduce or ... TODO: finish comments here
 * @author hugo benichi
 * @version 0.1.0
 */
public interface Stream<E> extends Iterable<E> {

    /**
     * Map this stream to another output stream by aplying a caller defined 
     * operation. New values are produced as needed when the output stream is
     * iterated over. The operation is specified by the caller as a Function
     * object and is typically an anonymous instance of the Function interface.
     * @param <F>       the type of the output Stream.
     * @param transform a Function object which specifies how the objects
     * provided by this stream should be transformed.
     * @return          a new Stream of possibly a different type.
     * @see Function
     */
    <F> Stream<F> map(Function<? super E,? extends F> transform);

    /**
     * Return a substream of this stream filtered from any object which does not
     * pass a called defined test. The test is provided as a Predicate object
     * and is typically an anonymous instance of the Predicate interface.
     * @param check a Predicate object which specifies which test to run.
     * @return      a new Stream object of the same type.
     * @see Predicate
     */
    Stream<E> select(Predicate<? super E> check);

    // TODO: shouldn t first param of Operator be ? extend E ?
    /**
     * Return a scalar value computed from all the values of this Stream. The
     * ouput is computed from a caller defined reducer operation specified as a
     * Operator object with matching accumulator and input types.
     * @param reducer a Operator object to compute the reduced scalar value.
     * @return        the final state of the reduce operation.
     * @see Operator
     */
    //<T super E> 
    Function<?,E> reduce(Operator<E,E> reducer);

    /**
     * TODO: DOCME!
     * @param <F>        the type of the output scalar value. May be different
     * from E.
     * @param folder     a Operator object to compute the reduced scalar value.
     * @param init_state the initial state of the returned scalar value.
     * @return           the final state of the fold operation.
     * @see Operator
     */
    <F> Function<F,F> fold(Operator<F,E> folder);

}
