public class Node {

  public static final Node First = new Node(null);
  public static final Node Last = new Node(null);

  private Node prev = First;
  private Node next = Last;
  public final String key;

  public Node(String key) {
    this.key = key;
  }

  public String remove() {
    link(prev, next);
    return key;
  }

  public Node prev() { return prev; }
  public Node next() { return next; }

  public static void link(Node left, Node right) {
    if (left != First)
      left.next = right;
    if (right != Last)
      right.prev = left;
  }

}
