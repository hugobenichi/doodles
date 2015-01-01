//package tree;

public class Node<E extends Comparable<? extends E>> {

    public final E value;
    protected Node<E> parent;
    protected Node<E> left;
    protected Node<E> right;

    public Node(E value, Node<E> parent){
        this.value = value;
        init_links(parent);
    }

    public Node(E value){
        this.value = value;
        init_links(null);
    }

    private void init_links(Node<E> parent) {
        this.parent = parent;
        this.left = null;
        this.right = null;
    }

    public void rotate_right(){
        if (this.left == null) return;
        this.left.parent = this.parent;
        this.parent = this.left;
        this.left = this.left.right;
        this.parent.right = this;
    }

    public void rotate_left() {
        if (this.right == null) return;
        this.right.parent = this.parent;
        this.parent = this.right;
        this.right= this.right.left;
        this.parent.left = this;
    }

}
