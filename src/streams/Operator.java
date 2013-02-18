package streams;

/**
 * Specify an interface for an Operator object to pass as argument to
 * Stream#reduce and Stream#fold.
 * @see Stream
 * @author hugo benichi
 * @version 0.1.0
 */
public interface Operator<F,E> {

    /**
     * method that callers of Stream#reduce and Stream#fold implement to
     * produce a scalar value from a Stream.
     * In the case of reduce, F is the same type parameters as E.
     * In the case of fold, F is the type of the accumulator value (left fold)
     * @param left_operand  object of type F accumulating the fold
     * operation
     * @param right_operand object of type E provided by the Stream iterator
     * @return      an object of type F updated by the input of the Stream
     * iterator
     * @see Stream#reduce
     * @see Stream#fold
     */
    F call(F left_operand, E right_operand);

}
