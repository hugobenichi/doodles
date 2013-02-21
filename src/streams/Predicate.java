package streams;

/**
 * Specify an interface for a Predicate type to pass as argument to
 * Stream#select. A Predicate instance for type E represents a boolean test to
 * be performed on instances of E. Typically such an instance is made via an
 * anonymous class.
 * @see Stream
 * @author hugo benichi
 * @version 0.1.1
 */
public interface Predicate<E> {

    /**
     * method that callers of Stream#select implement to filter a Stream and
     * remove unwanted objects. Null values should be handled appropriately.
     * @param input an object of type E provided by the Stream iterator.
     * @return      true if the input object passed the test. False otherwise.
     * @see Stream#select
     */
    boolean call(E input);

}
