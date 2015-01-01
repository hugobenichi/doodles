public final class FromSortedArray {
  private FromSortedArray() {}

  private static final Node LEAF = new Node() {
      public boolean isLeaf() { return true; }
      public Node left() { throw new IllegalStateException(); }
      public Node right() { throw new IllegalStateException(); }
  };

  static class IntNode implements Node {
      private final int val;
      private final Node left;
      private final Node right;
      public IntNode(int val) {
        this.val = val; this.left = LEAF; this.right = LEAF;
      }
      public IntNode(int val, Node l, Node r) {
        this.val = val; this.left = l; this.right = r;
      }
      public boolean isLeaf() { return false; }
      public Node left() { return left; }
      public Node right() { return right; }
  }

  public static Node fromArray(int[] ary) {
    return apply(ary, 0, ary.length-1);
  }

  public static Node apply(int[] ary, int from, int to) {
    if (from > to) return LEAF;
    if (from == to) return new IntNode(ary[from]);
    int pivot = (to + from) / 2;

    Node l = apply(ary, from, pivot-1);
    Node r = apply(ary, pivot+1, to);
    return new IntNode(ary[pivot], l, r);
  }
}
