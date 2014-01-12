/** Represents a group structure over a type T. */
public interface Bar<T> {
  /** Combine two elements of the group according to the group law.
   *  @param x an element of the group.
   *  @param y another element of the group.
   *  @return the result of applying the group law on the argument pair. */
  T combine(T x, T y);

  /** Returns the neutral element for the group law.
   *  @return the neutral element. */
  T neutral();

  /** Returns the opposite element with respect to the group law.
   *  @param x an element of the group.
   *  @return the opposite o the input element. */
  T opposite(T x);
}
