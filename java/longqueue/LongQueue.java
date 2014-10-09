import java.util.Arrays;

/** add methods to do putBytes and popBytes (ala ByteBuffer) */
public interface LongQueue {

  /** Returns the number of elements in this queue in constant time. */
  int size();

  /** Returns true if the queue contains no element, or false otherwise. */
  boolean isEmpty();

  /** Prepends an element in front of this queue in constant amortized time,
   *  effectively shifting the index of all other elements by +1. */
  void prepend(long l);

  /** Appends an element at the end of this queue in constant amortized time. */
  void append(long l);

  /** Equivalent to append(l), but name reflects a stack API.*/
  void push(long l);

  /** Returns and removes the last element of this queue in constant time. */
  long pop();

  /** Returns and removes the first element of this queue in constant time,
   *  effectively shifting the index of all other elements by -1. */
  long shift();

  /** Pops n items either from the front of this queue if n > 0 or from the end
   *  if n < 0. If n >= size(), then this queue becomes empty. */
  void drop(int n);

  /** Equivalent to prepend(l), but name reflects a queue API. */
  void enqueue(long l);

  /** Equivalent to pop(), but name reflects a queue API. */
  long dequeue();

  /** Set the value of element at index idx to l if the index is in bound. */
  void set(int idx, long l);

  /** Get the element at index idx if the index is in bound. */
  long get(int idx);

  //void foreach(LongConsumer block); iterate in order

}

class ArrayLongQueue implements LongQueue {

  private long[] backing_array;
  private int offset;
  private int size;

  ArrayLongQueue(int initial_capacity) {
    backing_array = new long[next_two_power(initial_capacity)];
    size = 0;
    offset = 0;
  }

  public int size() { return size; }

  public boolean isEmpty() { return size == 0; }

  public void prepend(long l) {
    ensureCapacity();
    dset(-1, l);
    size++;
    offset--;
  }

  public void append(long l) {
    ensureCapacity();
    dset(size++, l);
  }

  public void push(long l) { append(l); }

  public long pop() {
    long last = dget(--size);
    return last;
  }

  public long shift() {
    long first = dget(0);
    size--;
    offset++;
    return first;
  }

  public void drop(int n) { /* not implemented */ }

  public void enqueue(long l) { prepend(l); }

  public long dequeue() { return pop(); }

//  public void foreach(LongConsumer block) {
//    for (int i = 0; i < size; i++) { block.apply(get(i)); }
//  }

  public void set(int idx, long l) { check_bounds(idx); dset(idx, l); }

  public long get(int idx) { check_bounds(idx); return dget(idx); }

  private void check_bounds(int idx) {
    if (idx < 0 || idx >= size)
      throw new IndexOutOfBoundsException("size: " + size + " idx: " + idx);
  }

  private void dset(int idx, long l) {
    backing_array[position_of(idx)] = l;
  }

  private long dget(int idx) { return backing_array[position_of(idx)]; }

  private int position_of(int queue_idx) {
    int array_idx = ((1 << 30) + queue_idx + offset) & (backing_array.length - 1);

    assert 0 <= array_idx && array_idx < backing_array.length
      : "out of bound index into the ArrayLongQueue backing array";

    return  array_idx;
  }

  private void printAry() {
    System.out.print("[");
    for (int i = 0; i < backing_array.length; i++) {
      System.out.print(backing_array[i] + ", ");
    }
    System.out.println("]");
  }

  private void ensureCapacity() {
    if (size < backing_array.length) return;

    long[] new_array = new long[growing_policy(backing_array.length)];

    // rellocate memory starting at index 0; todo: use system copy function
    for (int i = 0; i < size; i++) { new_array[i] = get(i); }

    backing_array = new_array;
    offset = 0;
  }

  private static int growing_policy(int old_len) {
    int new_len = 2 * old_len;

    assert new_len <= 1 << 30 : "tried to grow backing array beyond 1 << 30";

    return new_len;
  }

  private static int next_two_power(int initial_capacity) {
    if (initial_capacity < 1) return 16;
    if (initial_capacity > (1 << 20)) return 1 << 20;

    int power = Integer.highestOneBit(initial_capacity);
    if (power != initial_capacity) power *= 2;

    assert (power & ~(power - 1)) == power
      : "initial required capacity could not be coerced into a power of two";

    return power;
  }

}

final class ArrayLongQueueTest {
  private ArrayLongQueueTest() { /* static class */ }

  public static void pushPopTest(int init, int n_op) {
    LongQueue q = new ArrayLongQueue(init);
    for (int i = 0; i < n_op; i++) {
      q.push(i);
      assert q.size() == (i + 1) : "size() != number of push()";
    }
    for (int i = n_op; i > 0; i--) {
      long x = q.pop();
      assert x == (i - 1) : "pop() did not return " + (i - 1) + " but " + x;
    }
  }

  public static void enqueueDequeueTest(int init, int n_op) {
    LongQueue q = new ArrayLongQueue(init);
    for (int i = 0; i < n_op; i++) {
      q.enqueue(i);
      assert q.size() == (i + 1) : "size() != number of enqueue()";
    }
    for (int i = 0; i < n_op; i++) {
      long x = q.dequeue();
      assert x == i : "dequeue() did not return " + i + " but " + x;
    }
  }

  public static void enqueueDequeueCycleTest(int working_size, int n_op) {
    LongQueue q = new ArrayLongQueue(working_size);
    for (int i = 0; i < working_size; i++) {
      q.enqueue(i);
      assert q.size() == (i + 1) : "size() != number of push()";
    }
    for (int i = working_size; i < working_size + n_op; i++) {
      long x = q.dequeue();
      assert x == (i - working_size)
        : "pop() did not return " + (i - working_size) + " but " + x;
      q.enqueue(i);
    }
  }

  public static void pushShiftCycleTest(int working_size, int n_op) {
    LongQueue q = new ArrayLongQueue(working_size);
    for (int i = 0; i < working_size; i++) {
      q.push(i);
      assert q.size() == (i + 1) : "size() != number of push()";
    }
    for (int i = working_size; i < working_size + n_op; i++) {
      long x = q.shift();
      assert x == (i - working_size)
        : "pop() did not return " + (i - working_size) + " but " + x;
      q.push(i);
    }
  }

  public static void queueCanGrowTest() {
    int[] sizes = new int[]{100, 1000, 10000, 100000};
    for (int s = 0; s < sizes.length; s++) {
      LongQueue q = new ArrayLongQueue(20);
      int size = sizes[s];
      for (int i = 0; i < size; i++) { q.push(i); }
      for (int i = 0; i < size; i++) {
        assert q.get(i) == i
          : "error at i=" + i + ": q[i] was not i but " + q.get(i);
      }
    }
  }

  public static void main(String[] argv) {
    enqueueDequeueTest(20,10);
    enqueueDequeueTest(20,100);
    enqueueDequeueTest(20,1000);
    enqueueDequeueCycleTest(20,100);
    enqueueDequeueCycleTest(20,1000);
    pushShiftCycleTest(20,100);
    pushShiftCycleTest(20,1000);
    pushPopTest(20,10);
    pushPopTest(20,100);
    pushPopTest(20,1000);
    queueCanGrowTest();
  }
}
