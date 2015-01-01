public class BinHeap {

  private int size;
  private Node root;

  public BinHeap() {
    this.size = 0; this.root = Node.leaf;
  }

  public void add(int x) {
    size++;
    if (size == 1)
      root = new Node(x);
    else
      root.addInLastPosition(x, sizeToBranching(size));
  }

  public int peek() { return root.val; }

  public int pop() {
    int min = peek();
    if (size == 1)
      root = Node.leaf;
    else
      root.delInLastPosition(sizeToBranching(size));
    size--;
    return min;
  }

  public int size() { return size; }

  public boolean isEmpty() { return size == 0; }

  private static int sizeToBranching(int size) {
    int rev = Integer.reverse(size);
    return rev >>> (1 + Integer.numberOfTrailingZeros(rev));
  }

}

class Node {

  public static final Node leaf = new Node(Integer.MIN_VALUE);

  public int val;
  public Node left;
  public Node right;

  public Node(int val) {
    this.val = val; this.left = leaf; this.right = leaf;
  }

  public void addInLastPosition(int x, int branching) {
    if (branching == 0) {
      assert left == Node.leaf;
      left = new Node(x);
    }
    if (branching == 1) {
      assert right == Node.leaf;
      right = new Node(x);
    }
    Node child = ((branching & 1) == 0) ? left : right;
    child.addInLastPosition(x, branching >> 1);
    ensureSmallerThan(child);
  }

  public void delInLastPosition(int branching) {
    if (branching == 0) {
      assert right == Node.leaf;
      swapValWith(left);
      left = Node.leaf;
      return;
    }

    if (branching == 1) {
      swapValWith(right);
      ensureSmallerThan(left);
      right = Node.leaf;
      return;
    }

    Node recurNode = left;
    Node otherNode = right;
    if ((branching & 1) == 1) {
      recurNode = right;
      otherNode = left;
    }
    swapValWith(recurNode);
    ensureSmallerThan(otherNode);

    recurNode.delInLastPosition(branching >>> 1);
  }

  private void ensureSmallerThan(Node that) {
    if (that.val > this.val)
      this.swapValWith(that);
  }

  private void swapValWith(Node that) {
    int temp = this.val;
    this.val = that.val;
    that.val = temp;
  }

}
