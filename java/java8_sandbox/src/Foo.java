/** Represents the addition group over the natural numbers. @see Bar .*/
public class Foo implements Bar<Integer> {
  public Integer combine(Integer x, Integer y) { return x + y; }
  public Integer neutral() { return 0; }
  public Integer opposite(Integer x) { return -x; }
}
