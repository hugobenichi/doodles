import java.lang.reflect.Array;
import java.util.Arrays;

public class Iteri {
  <X> Enumerator<X> from(X[] xs) {
    return new ArrayEnumerator<X>(xs);
  }
}

interface Enumerator<X> {
  X get();
  boolean hasNext();
  int next();
  int skip(int n);
  Throwable err();
}

final class ArrayEnumerator<X> implements Enumerator<X> {

  private X[] source;
  private int cursor;
  private int end;

  ArrayEnumerator(X[] xs) {
    source = xs;
    cursor = 0;
    end = xs.length;
  }

  public X get()            { return source[cursor]; }
  public boolean hasNext()  { return cursor < end; }
  public int next()         { cursor++; return hasNext() ? 0 : -1; }
  public int skip(int n)    { cursor += n; return hasNext() ? 0 : -1; }
  public Throwable err()    { /* not implemented */ return null; }
}

interface Iteratee<X,R> {
  int push(X x);
  //int push(X[] xs);
  Throwable err();
  int drop();
  R finish();
}

final class ArrayIteratee<X> implements Iteratee<X,X[]> {
  private X[] out;
  private int cursor;
  ArrayIteratee(int sizehint, Class<X> classtag) {
    @SuppressWarnings("unchecked")
    X[] xs = (X[])Array.newInstance(classtag, sizehint);
    out = xs;
  }
  private void ensurecapacity() {
    if (cursor < out.length) {
      return;
    }
    out = Arrays.copyOf(out, 2 * out.length);
  }
  public int push(X x)    { ensurecapacity(); out[cursor++] = x; return 0; }
  public X[] finish()     { return out; /* need to copy if size is wrong */}
  public int drop()       { return 0; }
  public Throwable err()  { return null; /* not supported */ }
}

interface Enumeratee<X,Y> {
  // give Iteratee of Y,R, return iteratee of X,R
  // give Enumerator of X, return Enumerator of Y

  Enumerator<Y> map(Enumerator<X> in);
  <R> Iteratee<X,R> into(Iteratee<Y,R> post);
}
