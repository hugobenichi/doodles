package streams;

/**
 * Specify an interface for a Function type to pass as argument to Stream#map.
 * Instances of Function for types E and F represents a computation taking as
 * input an instance of type E and returning an instance of type F.
 * @see Stream
 * @author hugo benichi
 * @version 0.1.3
 */
public interface Function<E,F> {

    /**
     * method that callers of Stream#map implement to transform a stream of
     * elements of type E to another stream with possibly a different type.
     * @param input object of type E provided by the Stream iterator.
     * @return      an object of type F.
     * @see Stream#map
     */
    F call(E input);

}
