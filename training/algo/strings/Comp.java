import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.LinkedList;

public class Comp {

  public static final String text = "This API is based on a constructor that takes as argument an R-character string that specifies the alphabet and the toChar() and toIndex() methods for converting (in constant time) between string characters and int values between 0 and R-1. It also includes a contains() method for checking whether a given character is in the alpha- bet, the methods R() and lgR() for finding the number of characters in the alphabet and the number of bits needed to represent them, and the methods toIndices() and toChars() for converting between strings of characters in the alphabet and int arrays. For convenience, we also include the built-in alphabets in the table at the top of the next page, which you can access with code such as Alphabet.UNICODE. Implementing Alphabet is a straightforward exercise (see Exercise 5.1.12). We will examine a sample client on page 699.";

  public static int commonLength(String s, int i, int j) {
    if (i == j) return s.length() - i;
    if (i > j) return commonLength(s, j, i);
    int len = 0;
    while (j < s.length() && s.charAt(i) == s.charAt(j)) { i++; j++; len++; }
    return len;
  }

  public static int match(String s, int offset, String t) {
    int i = 0;
    while (i + offset < s.length()) {
      if (i == t.length()) return 0;
      if (i + offset == s.length()) return -1;
      if (s.charAt(i+offset) != t.charAt(i)) break;
      i++;
    }
    return s.charAt(i + offset) - t.charAt(i);
  }

  public static int compInto(String s, int i, int j) {
    if (i == j) return 0;
    if (i > j) return - compInto(s, j, i);
    while (j < s.length() && s.charAt(i) == s.charAt(j)) { i++; j++; }
    if (j == s.length()) return -1;
    return s.charAt(i) - s.charAt(j);
  }

  public static int comp(String s, String t) {
    if (s.length() > t.length()) return - comp(t,s);
    int i = 0;
    while (i < s.length() && s.charAt(i) == t.charAt(i)) { i++; }
    if (i == s.length())
      return i == t.length() ? 0 : -1;
    return s.charAt(i) - t.charAt(i);
  }

  public static int[] suffixArray(final String s) {
    Integer[] suff = new Integer[s.length()];
    for (int i = 0; i < suff.length; i++) { suff[i] = i; }
    Comparator<Integer> comp = new Comparator<Integer>() {
      public int compare(Integer i, Integer j) { return compInto(s,i,j); }
    };
    Arrays.sort(suff, comp);
    int[] nativeSuff = new int[s.length()];
    for (int i = 0; i < suff.length; i++) { nativeSuff[i] = suff[i]; }
    return nativeSuff;
  }

  public static String longestSubstring(String s) {
    int[] suff = suffixArray(s);
    int bestLen = 0;
    int bestIdx = 0;
    for (int i = 0; i < suff.length -1; i++) {
      int len = commonLength(s, suff[i], suff[i+1]);
      if (len > bestLen) {
        bestLen = len;
        bestIdx = i;
      }
    }
    return s.substring(suff[bestIdx],suff[bestIdx]+bestLen);
  }

  public static int matchSearch(String text, int[] suffix, String t, int from, int to) {
    while (from <= to) {
      int mid = (from + to)/2;
      int comp = match(text, suffix[mid], t);
      System.out.println(mid + ":" + comp);
      if (comp == 0) return mid;
      if (comp > 0) {
        to = mid -1;
      } else {
        from = mid + 1;
      }
    }
    return -1;
  }

  public static Iterable<String> matches(String text, int[] suffix, String t) {
    List<String> matching = new LinkedList<>();
    int idx = matchSearch(text, suffix, t, 0, suffix.length-1);
System.out.println(idx);
//System.out.println(text.substring(suffix[idx]));
    if (idx >= 0) {
      int from = idx;
      int to = idx;
      while (match(text, suffix[from-1], t) == 0) { from--; }
      while (match(text, suffix[to+1], t) == 0) { to++; }
      for (int i = from; i <= to; i++) {
        matching.add(text.substring(suffix[i] - 6, suffix[i] + t.length() + 6));
      }
    }
    return matching;
  }

  public static void main(String[] argv) {
  /*
    int i = Integer.parseInt(argv[1]);
    int j = Integer.parseInt(argv[2]);
    System.out.println(compInto(argv[0], i, j));
  */

    //System.out.println(longestSubstring(argv[0]));

    int[] suff = suffixArray(text);
    for (String s : matches(text, suff, argv[0])) { System.out.println(s); }

  }
}
