public final class Balance {
  private Balance() {}

  public static boolean isBalance(Node n) { return balance(n) >= 0; }

  public static int balance(Node n) {
    if (n.isLeaf()) return 0;

    int left_depth = balance(n.left());
    if (left_depth < 0) return -1;

    int right_depth = balance(n.right());
    if (right_depth < 0) return -1;

    if (Math.abs(right_depth-left_depth) > 1) return -1;

    return Math.max(right_depth,left_depth);
  }

}
