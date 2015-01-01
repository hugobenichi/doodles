/* TODO - Make the number of milliseconds printed below as small as you can.

  Question 2.1. Using a single thread.
    We notice that the numbers found in the array are taken from the range
    [0,9]. Instead of using a O(n x log(n)) sorting algorithm like the system
    sort, we can use a special case of bucket sort by simply counting how many
    times each numbers in the range [0,9] appears in the array. We keep track
    if this count statistics in a length 10 array of int. After traversing the
    array to sort, we iterate through the count statistics and write in the
    array starting from index 0 as many occurrences of a number as we have seen.

  Question 2.2. Using K threads.
    The count statistics has the property that it is independent of the
    actual positions in the array of the numbers we are counting (the
    count statistics is invariant by permutations of elements).
    Therefore we can count independently non-overlapping segments of the
    array and merge the count statistics by adding the counts of a given
    number together. Conceptually this is an application of the "map reduce"
    workflow. This gives a straightforward way to parallelize the counting
    algorithm with minimal synchronisation. Furthermore, by dividing the
    array into K equal segments, we naturally get a balanced workload for
    K threads where each location in the array is visited by a single thread
    exactly once.

  Question 2.3. For extra credit - can you explain why the performance
    improvement you see with K threads isn't nearly as high as you'd expect?

    On my system (i7 2 cores @ 2 GHz), the system sort takes around 2200 ms.
    The serial count sort takes around 190 ms, with 110 ms for counting.

    The parallel count sort with two threads takes around 140 ms. Only the
    counting step is parallel, and takes about 65 ms, which is a bit less than
    a 2x speedup. The overhead of creating and joining the worker threads
    compared to the total amount of work explains why a 2x speedup can't be
    achieved. With 4 threads, the time for counting improves a bit and falls
    to 55ms, which is well bellow the theoretical 4x speedup.

    Overall, the speedup compared to the serial version is about 35%. If the
    rewriting step is also parallelized, with two threads the speedup is very
    marginal.
*/

import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;

public class Sorter {

  public void sort(int[] in) {
    Arrays.sort(in);
  }

  public static void main(String[] args) throws Exception {
    for (int nRun = 0; nRun < 3; nRun++) {

      final int[] input = new int[1<<27];

      Random rand = new Random();
      for(int i = 0;i < input.length;i++) {
        input[i] = Math.abs(rand.nextInt())%10;
      }

      //Sorter sorter = new Sorter();
      //CountingSorter sorter = new CountingSorter();
      ParCountingSorter sorter = new ParCountingSorter(2);

      int[] c1 = sorter.count(input,10);

      long t0 = System.nanoTime();
      sorter.sort(input);
      long t1 = System.nanoTime();

      System.out.println("Sorting took " + (t1-t0)/1000000 + " millis");

      // check the array is sorted
      for (int i = 1; i < input.length; i++) { assert input[i-1] <= input[i]; }

      // check the count statistics invariance by permutation.
      int[] c2 = sorter.count(input,10);
      for (int i = 0; i < c1.length; i++) { assert c1[i] == c2[i]; }
    }
  }

}

abstract class BaseCountingSorter extends Sorter {

  public void sort(int[] in) {

    long t0 = System.nanoTime();

    int[] counts = count(in, 10);

    long t1 = System.nanoTime();

    writeInto(in, counts);

    long t2 = System.nanoTime();

    System.out.println("counting: " + (t1 - t0) / 1000000 + " millis");
    System.out.println("writing: " + (t2 - t1) / 1000000 + " millis");
  }

  abstract public int[] count(int[] in, int max);

  public void writeInto(int[] ary, int[] counts) {
    int idx = 0;
    for (int num = 0; num < counts.length; num++) {
      for (int n = 0; n < counts[num]; n++) {
        ary[idx++] = num;
      }
    }
  }

  protected static int[] countInRange(int[] ary, int from, int to, int max) {
    int[] counts = new int[max];
    int stop = Math.min(to+1, ary.length);
    for (int i = from; i < stop; i++) {
      counts[ary[i]] += 1;
    }
    return counts;
  }

}

class CountingSorter extends BaseCountingSorter {

  public int[] count(int[] in, int max) {
    return countInRange(in, 0, in.length-1, max);
  }

}

class ParCountingSorter extends BaseCountingSorter {

  private final int nThread;

  ParCountingSorter(int nThread) { this.nThread = nThread; }

  public int[] count(int[] in, final int max) {

    final int[] ary = in;
    final int[] segments = new int[nThread+1];
    final int[][] counts = new int[nThread][];

    // Divide the array into nThread workloads.
    for (int i = 0; i < nThread; i++) {
      segments[i] = i * (ary.length / nThread);
    }
    segments[nThread] = ary.length;

    final AtomicInteger id = new AtomicInteger();
    Thread[] workers = new Thread[nThread];

    // Prepare the worker threads and start them. They will grab a workload by
    // getting a value from AtomicInteger which acts as a monotically increasing
    // counter.
    for (int i = 0; i < nThread; i++) {
      workers[i] = new Thread(
        new Runnable() {
          public void run() {
            try {
              int segmentNum = id.getAndIncrement();
              int from = segments[segmentNum];
              int to = segments[segmentNum+1] - 1;
              counts[segmentNum] = countInRange(ary, from, to, max);
            } catch (Exception e) { e.printStackTrace(); }
          }
        }
      );
      workers[i].start();
    }

    // Wait for the workers to finish they job.
    for (int i = 0; i < nThread; i++) {
      try {
        workers[i].join();
      } catch (InterruptedException ignored) { }
    }

    // Merge the results
    int[] mergedCounts = new int[max];
    for (int j = 0; j < nThread; j++) {
      for (int i = 0; i < max; i++) {
        mergedCounts[i] += counts[j][i];
      }
    }

    return mergedCounts;
  }

  public void parWriteInto(final int[] ary, final int[] counts) {

    final AtomicInteger id = new AtomicInteger();
    final int[] segments = new int[nThread+1];
    for (int i = 0; i < nThread; i++) {
      segments[i] = i * (counts.length / nThread);
    }
    segments[nThread] = counts.length;

    Thread[] workers = new Thread[nThread];
    for (int i = 0; i < nThread; i++) {
      workers[i] = new Thread(
        new Runnable() {
          public void run() {
            try {
              int segmentNum = id.getAndIncrement();
              int from = segments[segmentNum];
              int to = segments[segmentNum+1];
              int idx = 0;
              for (int num = 0; num < from; num++) {
                idx += counts[num]; // calculate offset
              }
              for (int num = from; num < to; num++) {
                for (int n = 0; n < counts[num]; n++) {
                  ary[idx++] = num;
                }
              }
            } catch (Exception e) { e.printStackTrace(); }
          }
        }
      );
      workers[i].start();
    }

    for (int i = 0; i < nThread; i++) {
      try {
        workers[i].join();
      } catch (InterruptedException ignored) { }
    }
  }

}
