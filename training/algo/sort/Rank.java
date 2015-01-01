import java.util.Random;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public final class Rank {

  private static final int nThreads = 4;
  private static final int batchSize = 10000;
  private static final ForkJoinPool scheduler = new ForkJoinPool(nThreads);
  private static final AtomicInteger task = new AtomicInteger();

  private static final Random r = new Random();

  public static int get(int upto) { return Math.abs(r.nextInt()) % upto; }

  public static void swap(int[] ary, int i, int j) {
    int s = ary[i];
    ary[i] = ary[j];
    ary[j] = s;
  }

  public static int pivot(int[] ary, int from, int to) {
    int idx = from + get(1 + to - from);
    int piv = ary[idx];
    swap(ary, idx, to);
    idx = partition(ary, from, to-1, piv);
    swap(ary, idx, to);
//verifyPivot(ary, from, to, idx);
    return idx;
  }

  public static int partition(int[] ary, int from, int to, int pivot) {
    while (true) {
      while (from <= to && ary[from] < pivot) from++;
      while (from <= to && ary[to] >= pivot) to--;
      if (from > to) return from;
      swap(ary, from, to);
    }
  }

  public static void ranksort(int[] ary, int from, int to) {
    if (from >= to) {
      task.getAndDecrement();
      return;
    }
    if (from + 1 == to) {
      if (ary[from] > ary[to]) swap(ary, from, to);
      task.getAndDecrement();
      return;
    }

    int split_point = pivot(ary, from, to);

    task.getAndIncrement();
    ranksort(ary, from, split_point-1);
    ranksort(ary, split_point+1, to);
  }

  public static void ranksortP(int[] ary, int from, int to) {
    if (from >= to) {
      task.getAndDecrement();
      return;
    }
    if (from + 1 == to) {
      if (ary[from] > ary[to]) swap(ary, from, to);
      task.getAndDecrement();
      return;
    }

    int split_point = pivot(ary, from, to);

    task.getAndIncrement();
    if (to - from < batchSize) {
      ranksort(ary, from, split_point-1);
      ranksort(ary, split_point+1, to);
    } else {
      scheduler.execute(defered(ary, from, split_point-1));
      scheduler.execute(defered(ary, split_point+1, to));
    }
  }

  public static Runnable defered(final int[] ary, final int from, final int to) {
    return new Runnable() { public void run() { ranksortP(ary, from, to); } };
  }

  public static int maxIn(int[] ary, int from, int to) {
    int max = ary[from];
    for (; from <= to; from++) { if (ary[from] > max) max = ary[from]; }
    return max;
  }

  public static int minIn(int[] ary, int from, int to) {
    int min = ary[from];
    for (; from <= to; from++) { if (ary[from] < min) min = ary[from]; }
    return min;
  }

  public static void verifyPivot(int[] ary, int from, int to, int pivot) {
    assert pivot > to || maxIn(ary, from, pivot-1) <= minIn(ary, pivot, to);
  }

  public static void verifySorted(int[] ary, int from, int to) {
    for (int i = from+1; i <= to; i++) { assert ary[i-1] <= ary[i]; }
  }

  public static void printAry(int[] ary) {
    for (int i = 0; i < ary.length; i++) {
      System.out.print(ary[i]);
      System.out.print(" ");
    }
    System.out.println();
  }

  public static void main(String[] argv) {
    Random r = new Random();

    int len = argv.length > 0 ? Integer.parseInt(argv[0]) : 1 + get(10);

    int[] ary = new int[len];
    for (int i = 0; i < len; i++) { ary[i] = get(len); }
    //printAry(ary);

    task.getAndIncrement();
    long start = System.nanoTime();
    ranksortP(ary, 0, len-1);
    try {
      while (task.get() != 0) { Thread.sleep(100); }
    } catch (Exception e) {}
    long stop = System.nanoTime();
    System.out.println((stop-start)/1000000);
    verifySorted(ary, 0, len-1);

  }

}
