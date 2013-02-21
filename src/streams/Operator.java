package streams;

/**
 * Specify an interface for an Operator type to pass as argument to
 * Stream#reduce and Stream#fold. An Operator instance for types F and E
 * represents an update of the state of an instance of type F according to the
 * instance of type E.
 * @see Stream
 * @author hugo benichi
 * @version 0.1.1
 */
public interface Operator<F,E> {

    /**
     * method that callers of Stream#reduce and Stream#fold implement to
     * produce a scalar value of type F from a Stream of type E. In the case of
     * reduce, F is the same type parameters as E. In the case of fold, F is the
     * type of the accumulator value (left fold)
     * @param left_operand  object of type F which is updated by the Operator
     * instance
     * @param right_operand object of type E provided by the Stream iterator and
     * consumed to update the 
     * @return      the updated left operand. Can be a new reference to a
     * different instance of F for immutable objects.
     * @see Stream#reduce
     * @see Stream#fold
     */
    F call(F left_operand, E right_operand);

}
