package streams;

/**
 * Specify an interface for a Predicate object to pass as argument to
 * Stream#select.
 * @see Stream
 * @author hugo benichi
 * @version 0.1.0
 */
public interface Predicate<E> {

    /**
     * method that callers of Stream#select implement to filter a Stream.
     * Typically with an anonymous Predicate object.
     * @param input an object of type E which is provided by the Stream
     * iterator.
     * @return      a boolean value which says if the input object passed the
     * test.
     * @see Stream#select
     */
    boolean call(E input);

}
