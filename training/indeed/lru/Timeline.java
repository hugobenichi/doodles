import java.util.HashMap;
import java.util.Map;

public class Timeline {

  private final Map<String,Node> cache;
  private Node oldest;
  private Node newest;
  private int  size;
  private final int capacity;

  public Timeline(int capacity) {
    this.capacity = capacity;
    this.size = 0;
    this.cache = new HashMap<>();
    this.oldest = Node.First;
    this.newest = Node.Last;
  }

  public int size() { return size; }

  public String push(String key) {
    Node node = cache.get(key);
    if (node == null)
      addNewEntry(key);
    else
      updateEntry(node);
    String oldRequest = ensureSize();
    return oldRequest;
  }

  private void addNewEntry(String key) {
    size++;
    Node.link(this.newest, new Node(key));
    this.newest = this.newest.next();
    cache.put(key, this.newest);
  }

  private void updateEntry(Node node) {
    size--;
    addNewEntry(node.remove());
  }

  private String ensureSize() {
    if (size <= capacity) {
      return null;
    } else {
      size--;
      Node toForget = this.oldest;
      this.oldest = this.oldest.next();
      return toForget.remove();
    }
  }

}
