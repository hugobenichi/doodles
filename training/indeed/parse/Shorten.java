import java.lang.StringBuilder;

public class Shorten {

  public static String shorten(String s, int len) {
    if (s.length() <= len) return s;
    StringBuilder bld = new StringBuilder();
    for (int index = 0; index < len || dontStopAt(s, index); index++) {
      bld.append(s.charAt(index));
    }
    return bld.toString();
  }

  public static boolean dontStopAt(String s, int index) {
    return !Character.isWhitespace(s.charAt(index));
  }

}
