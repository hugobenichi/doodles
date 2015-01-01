import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;

public interface Graph {
  String value();
  Iterable<Graph> nodes();

  static class Node implements Graph {

    private final String value;
    private final Set<Node> nodes = new HashSet<>();

    public Node(String v) { this.value = v; }

    public String value() { return value; }
    public Iterable<Graph> nodes() { return nodes; }
    public void link(Node n) { nodes.add(n); }
    public void unlink(Node n) { nodes.remove(n); }

    public Iterable<Node> pathTo(Node target) {
      Set<Node> visited = new HashSet<>();
      List<Node> toVisit = new LinkedList<>(nodes);
      Map<Node,Node> backtrace = new HashMap<>();

      while (!toVisit.isEmpty()) {
        Node n = toVisit.pop();
        if (n == target)
          return pathFromBackTrace(backtrace, n);
        if (visited.contains(n))
          continue;
        visited.add(n);
        for (Node m : n.nodes()) {
          if (!visited.contains(m)) {
            backtrace.put(m,n);
            toVisit.push(m);
          }
        }
      }
      return new LinkedList<Node>();
    }

    private Iterable<Node> pathFromBackTrace(Map<Node,Node> backtrace, Node n) {
      return new LinkedList<Node>();
    }

  }

}
