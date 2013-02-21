package streams;

import java.lang.Iterable;
import java.util.List;

import streams.Function;
import streams.Operator;
import streams.Predicate;

/**
 * Stream defines a wrapper for the Iterable interface and adds four useful
 * functions to apply and chain operations on iterators. All operations should
 * be conducted in a lazy fashion: values are computed only when necessary until
 * some outside iteration needs them. The preferred way to build Stream objects
 * is via the static factory method Streams#from.
 * @author hugo benichi
 * @version 0.1.2
 * @see Streams#from
 */
public interface Stream<E> extends Iterable<E> {

    /**
     * Maps this stream to another stream by applying a caller defined 
     * operation. New values are produced as needed when the output stream is
     * iterated over. The operation is specified by the caller as a Function
     * object and is typically an anonymous instance of the Function interface.
     * @param <F>       the type of the output Stream.
     * @param transform a Function object which specifies how the objects
     * provided by this stream should be transformed.
     * @return          a new Stream object, possibly of a different type.
     * @see Function
     */
    <F> Stream<F> map(final Function<? super E,? extends F> transform);

    /**
     * Returns a substream of this stream filtered from any object which does
     * not pass the boolean test represented by the caller given Predicate
     * object. Objects which fail the test are ignored by the stream iterator
     * and skipped. Therefore the stream iterator is typically one object ahead
     * in order to be able to check if there are any remaining valid objects in
     * the Stream.
     * @param check a Predicate object specifying the boolean test to run on
     * objects provided by the stream.
     * @return      a new Stream object of the same type.
     * @see Predicate
     */
    Stream<E> select(final Predicate<? super E> check);

    /**
     * Returns a Function which can compute a scalar value from all the values
     * of this Stream. The computation is defined by the caller with a reducer
     * operation specified as an Operator object with matching accumulator
     * (left operand) and input (right operand) types. Instead of directly
     * returning the scalar value, a callable object is returned to delay the 
     * actual computation as long as possible.
     * @param reducer an Operator object to compute the reduced scalar value.
     * @return        a Function object which when its call() method is invoked
     * returns the result of the reduce operation. The Function object takes any
     * value as input and can be called with null. The result is not buffered
     * and further invocations of call() will rerun the computation.
     * @see Operator
     * @see Function
     */
    Function<?,E> reduce(final Operator<E,E> reducer);

    /**
     * Returns a Function which can compute a scalar value all the values of
     * this Stream. The computation is defined by the caller with a fold 
     * operation specified as an Operator object with non-matching accumulator
     * (left operand) and input (right operand) types. Instead of directly
     * returning the scalar value, a callable object is returned to delay the 
     * actual computation as long as possible.
     * @param <F>        the type of the output scalar value, in general
     * different from E.
     * @param folder     an Operator object to compute the folded scalar value.
     * @return           a Function object which when its call() method is
     * invoked returns the result of the reduce operation. The input argument to
     * call() is the initial value of the fold operation. The result is not
     * buffered and further invocations of call() will rerun the computation.
     * @see Operator
     * @see Function
     */
    <F> Function<F,F> fold(final Operator<F,E> folder);

}
