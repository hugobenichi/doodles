package streams;

/**
 * Specify an interface for a Function object to pass as argument to Stream#map.
 * @see Stream
 * @author hugo benichi
 * @version 0.1.0
 */
public interface Function<E,F> {

    /**
     * method that callers of Stream#map implement to transform a stream of
     * elements of type E to another stream with possibly a different type.
     * @param input object of type E provided by the Stream iterator.
     * @return      an object of type F.
     * @see Stream#map
     */
    E call(F input);

}
