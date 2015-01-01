import java.util.*;
import java.io.*;


/*
 *  Cracking the coding interview, chap4, trees and graphs
 */
public class Graph<E> {

    public static final PrintStream stdout = System.out;

    private Map<E,Set<E>> links;

    public Graph(){ links = new HashMap<E,Set<E>>(); }

    public Iterable<E> nodes(){ return links.keySet(); }
    public Iterable<E> neighboors(E node) { return links.get(node);}

    public void add_node(E node) {
        if ( ! this.links.containsKey(node) ) this.links.put(node, new HashSet<E>());
    }

    public void add_ulink(E from, E to) {
        this.add_node(from);
        this.add_node(to);
        this.links.get(from).add(to);
    }

    public void add_blink(E from, E to) {
        this.add_ulink(from, to);
        this.links.get(to).add(from);
    }


    /*
     *  4.2 tells if there is a route from node A to node B
     */
    public boolean is_there_route(E from, E to) {
        return false;
    }

    public static void main(String argv[]) {

        System.exit(0);
    }

}
