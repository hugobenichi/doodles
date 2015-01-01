import java.util.*;

public class Path {

  public static void main(String[] argv) {
    int s = Integer.parseInt(argv[0]);
    int o = Integer.parseInt(argv[1]);
    path(s,o);
    //maze(s);
  }

  public static void path(int s, int o) {
    Grid gd = Grid.maze(s);
    //Grid gd = Grid.land(s,o);
    Graph<Long> g = gd.toGraph();

    //Graphs.printGraph(g);

    long a = gd.randomCell();
    long b = gd.randomCell();
    while (a == b) {
      b = gd.randomCell();
    }

/*
    Iterable<Long> path = Graphs.path(g, a, b);
    for (long l : path) {
      gd.set(l, '*');
    }
    */
    gd.set(b, 'X');

    Iterable<Long> bestPath = Graphs.astar(gd, g, a, b);
    //Iterable<Long> bestPath = Graphs.dijkstra(gd, g, a, b);
    for (long l : bestPath) {
      if (gd.get(l) == '*') {
        gd.set(l, 'o');
      } else {
        gd.set(l, '+');
      }
    }

    gd.set(b, 'X');
    //gd.set(a, 'X');

    System.out.println(gd);
  }

  public static void maze(int s) {
    System.out.println(Grid.maze(s));
  }

}

class Grid {

  public static final char dot = 0x0000;
  public static final char obs = 0x25FC;

  private static Random r = new Random();

  private final int size;
  private final char[][] cells;

  public Grid(int size) {
    this.size = size;
    this.cells = new char[size][size];
    for (Long c : allCells()) { set(c, ' '); }
  }

  public static <X> ArrayList<X> shuffle(ArrayList<X> ary) {
    for (int p = ary.size()-1; p >= 0; p--) {
      if (p == 0) break;
      int swap = r.nextInt(p);
      X temp = ary.get(p);
      ary.set(p, ary.get(swap));
      ary.set(swap, temp);
    }
    return ary;
  }

  public static Grid land(int size, int obstacles) {
    Grid gd = new Grid(size);
    //for (int i = 0; i < obstacles; i++) { set(randomCell(), obs); }
    for (int i = 0; i < obstacles; i++) {
      ArrayList<Long> points = new ArrayList<>();
      points.add(gd.randomCell());
      int obsSize = 2 + r.nextInt(10);
      for (int j = 0; j < obsSize; j++) {
        if (points.isEmpty()) break;
        for (Long f : shuffle(points)) {
          gd.set(f, obs);
          for (Long h : gd.neighbOf(f)) { points.add(h); }
          break;
        }
      }
    }
    return gd;
  }

  public static Grid maze(int size) {
    Grid gd = new Grid(size);
    Iterable<Long> cells = gd.allCells();
    for (long l: cells) { gd.set(l, Grid.obs); }
    Set<Long> visited = new HashSet<Long>();
    LinkedList<Long> todo = new LinkedList<Long>();
    LinkedList<Long> tobreak = new LinkedList<Long>();
    for (Long l: cells) { todo.push(l); tobreak.push(l); break; }

    while(!todo.isEmpty()) {
      long c = todo.pop();
      long w = tobreak.pop();
      if (!gd.isValid(c) || visited.contains(c)) {
        if (gd.isValid(w) && r.nextInt(100) > 90) gd.set(w,' ');
        continue;
      }
      gd.set(c,' ');
      gd.set(w,' ');
      visited.add(c);
      ArrayList<Long> next = gd.allNextNextLongs(c);
      ArrayList<Long> wall = gd.allNextLongs(c);
      int j = r.nextInt(4);
      for (int i = 0; i < 4; i++, j++) {
        todo.push(next.get(j % 4));
        tobreak.push(wall.get(j % 4));
      }
    }
    return gd;
  }

  public String toString() {
    StringBuilder bld = new StringBuilder();
    addHorSep(bld, size);
    for (char[] row : cells) { addRow(bld, row); }
    addHorSep(bld, size);
    return bld.toString();
  }

  private static void addRow(StringBuilder bld, char[] row) {
    bld.append("| ");
    for (char c : row) { bld.append(c).append(' '); }
    bld.append("|\n");
  }

  private static void addHorSep(StringBuilder bld, int len) {
    bld.append("+-");
    for (int i = 0; i < len; i++) { bld.append("--"); }
    bld.append("+\n");
  }

  public boolean isValid(long cell) {
    int x = (int)(cell>>>32);
    int y = (int)cell;
    return 0 <= x && x < size && 0 <= y && y < size;
  }

  public char get(long cell) { return cells[(int)(cell>>>32)][(int)cell]; }

  public void set(long cell, char c) { cells[(int)(cell>>>32)][(int)cell] = c; }

  public long randomCell() {
    for (;;) {
      long cell = cellOf(r.nextInt(size), r.nextInt(size));
      if (get(cell) != obs)
        return cell;
    }
  }

  public Graph<Long> toGraph() {
    HashMap<Long,Iterable<Long>> graph = new HashMap<Long,Iterable<Long>>();
    for(Long c : allCells()) { graph.put(c, shuffle(neighbOf(c))); }
    return new HashGraph<Long>(graph);
  }

  public Iterable<Long> allCells() {
    List<Long> allCells = new ArrayList<>(size*size);
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        if (cells[i][j] != obs) {
          allCells.add(cellOf(i,j));
        }
      }
    }
    return allCells;
  }

  private ArrayList<Long> neighbOf(long c) {
    ArrayList<Long> ls = new ArrayList<>(4);
    for (Long cell : allNextLongs(c)) {
      if (isValid(cell) && get(cell) != obs) ls.add(cell);
    }
    return ls;
  }

  private static ArrayList<Long> allNextLongs(long c) {
    int i = (int)(c >>> 32);
    int j = (int)c;
    ArrayList<Long> ls = new ArrayList<>(4);
    ls.add(cellOf(i-1,j));
    ls.add(cellOf(i,j-1));
    ls.add(cellOf(i+1,j));
    ls.add(cellOf(i,j+1));
    return ls;
  }

  private static ArrayList<Long> allNextNextLongs(long c) {
    int i = (int)(c >>> 32);
    int j = (int)c;
    ArrayList<Long> ls = new ArrayList<>(4);
    ls.add(cellOf(i-2,j));
    ls.add(cellOf(i,j-2));
    ls.add(cellOf(i+2,j));
    ls.add(cellOf(i,j+2));
    return ls;
  }

  private static long cellOf(int x, int y) {
    long cell = Integer.MAX_VALUE & x;
    return (cell << 32) | (Integer.MAX_VALUE & y);
  }

  public static int dist(long a, long b) {
    int ax = (int) a;
    int ay = (int) (a>>>32);
    int bx = (int) b;
    int by = (int) (b>>>32);
    return (int) Math.sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by));
  }

}

interface Graph<X> {
  boolean has(X x);
  Iterable<X> neighb(X x);
  Iterable<X> nodes();
}

class HashGraph<X> implements Graph<X> {
  private final Map<X,Iterable<X>> links;
  public HashGraph(Map<X,Iterable<X>> links) { this.links = links; }
  public boolean has(X x) { return links.containsKey(x); }
  public Iterable<X> neighb(X x) { return links.get(x); }
  public Iterable<X> nodes() { return links.keySet(); }
}

class Graphs {

  public static <X> Iterable<X> path(Graph<X> g, X start, X end) {
    Map<X,X> backmap = new HashMap<X,X>();
    List<X> toVisit = new LinkedList<X>();
    toVisit.add(start);
    while(!toVisit.isEmpty()) {
      X node = toVisit.remove(0);
      if (node.equals(end)) {
        return pathFromBackmap(start, end, backmap);
      }
      ArrayList<X> ls = new ArrayList<>(4);
      for (X neigh : g.neighb(node)) { ls.add(neigh); }
      Grid.shuffle(ls);
      //for (X neigh : g.neighb(node)) {
      for (X neigh : ls) {
        if (!backmap.containsKey(neigh)) {
          backmap.put(neigh, node);
          toVisit.add(neigh);
        }
      }
    }
    return Collections.emptyList();
  }

  public static Iterable<Long> dijkstra(Grid gd, Graph<Long> g, long start, long end) {
    Map<Long,Integer> dist = new HashMap<>();
    Map<Long,Long> backmap = new HashMap<Long,Long>();
    List<Long> toVisit = new LinkedList<Long>();
    toVisit.add(start);
    dist.put(start,0);
    while(!toVisit.isEmpty()) {
      Long node = toVisit.remove(0);
      int d = dist.get(node);
      gd.set(node, '.');
      if (node == start)
        gd.set(node, 'X');
      System.out.println(gd);
      try { Thread.sleep(20); } catch (Exception e) {}
      if (node.equals(end)) {
        //for (Long c : dist.keySet()) { gd.set(c, (char)('0' + dist.get(c))); }
        //for (Long c : dist.keySet()) { gd.set(c, '.'); }
        return pathFromBackmap(start, end, backmap);
      }
      for (Long neigh : g.neighb(node)) {
        if (!backmap.containsKey(neigh) || dist.get(neigh) > d+1) {
          backmap.put(neigh, node);
          toVisit.add(neigh);
          dist.put(neigh,d+1);
        }
      }
    }
    return Collections.emptyList();
  }

  public static Iterable<Long> astar(Grid gd, Graph<Long> g, long start, long end) {
    Map<Long,Integer> dist = new HashMap<>();
    Map<Long,Long> backmap = new HashMap<Long,Long>();

    PriorityQueue<Long> nodeQueue = new PriorityQueue<>(64, astarComp(end));
    nodeQueue.add(start);

    dist.put(start,0);
    int bestDist = Grid.dist(start, end);
    while(!nodeQueue.isEmpty()) {
      Long node = nodeQueue.poll();
      gd.set(node, '.');
      if (node == start)
        gd.set(node, 'X');
      System.out.println(gd);
      try { Thread.sleep(20); } catch (Exception e) {}
      int d = dist.get(node);
      if (node.equals(end)) {
        //for (Long c : dist.keySet()) { gd.set(c, '.'); }
        return pathFromBackmap(start, end, backmap);
      }
      for (Long neigh : g.neighb(node)) {
        if (!backmap.containsKey(neigh) || dist.get(neigh) > d+1) {
          backmap.put(neigh, node);
          int nd = Grid.dist(neigh, end);
          nodeQueue.add(neigh);
          dist.put(neigh,d+1);
        }
      }
    }
    return Collections.emptyList();
  }

  public static Comparator<Long> astarComp(final long dest) {
    return new Comparator<Long>() {
      public int compare(Long a, Long b) {
        return Integer.compare(Grid.dist(a,dest),Grid.dist(b,dest));
      }
      public boolean equals(Object obj) { return false; }
    };
  }

  public static <X> Iterable<X> pathFromBackmap(X start, X end, Map<X,X> map) {
    List<X> path = new LinkedList<X>();
    while (!end.equals(start)) {
      X prev = map.get(end);
      path.add(0, prev);
      end = prev;
    }
    path.remove(0);
    return path;
  }

  public static void printGraph(Graph<Long> g) {
    for (Long n : g.nodes()) {
      System.out.print(cellStr(n));
      System.out.print(" -> ");
      for (Long m : g.neighb(n)) {
        System.out.print(cellStr(m));
        System.out.print(", ");
      }
      System.out.println();
    }
  }

  public static String cellStr(long cell) {
    int x = (int) (cell >>> 32);
    int y = (int) cell;
    return "(" + x + "," + y + ")";
  }

}
