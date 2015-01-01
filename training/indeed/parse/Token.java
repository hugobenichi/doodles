import java.util.StringTokenizer;

public class Token {

  public static void main(String[] argv) {

    String[] tokens = "this is a test".split("\\s");

    for (String s : tokens) { System.out.println(s); }

  }

}
