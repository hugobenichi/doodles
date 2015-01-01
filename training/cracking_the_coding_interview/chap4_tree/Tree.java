import java.util.*;
import java.io.*;

public class Tree<E> {

    public static final PrintStream stdout = System.out;

    public Tree<E> parent, left, right;
    public E value;
    
    public Tree(E v) { this.value = v; this.left = this.right = this.parent = null;}
    public Tree(E value, Tree<E> parent) { this(value); this.parent = parent;}

    public String toString() { return this.value.toString(); }

    public List<List<Tree<E>>> nodes_hierarchy() {
        List<List<Tree<E>>> nodes = new LinkedList<List<Tree<E>>>();
    
        List<Tree<E>> current = new LinkedList<Tree<E>>();
        current.add(this);

        while (current.isEmpty() == false) {
            nodes.add(current);
            List<Tree<E>> next = new LinkedList<Tree<E>>();

            for (Tree<E> node : current) {
                if( node.left != null) next.add(node.left);
                if( node.right != null) next.add(node.right);
                current = next;
            }
        }

        return nodes;
    }

    public boolean is_bst(Comparator<? super E> comp) {
        E min = this.first().value;
        E max = this.last().value;
        return is_bst(min, max, comp);
    }

    public Tree<E> first() {
        Tree<E> most_left = this;
        while (most_left.left != null) most_left = most_left.left;
        return most_left;
    }

    public Tree<E> last() {
        Tree<E> most_right = this;
        while (most_right.right != null) most_right = most_right.right;
        return most_right;
    }

    public boolean is_bst(E min, E max, Comparator<? super E> comp) {

        if ( comp.compare(this.value, min) < 0) /* is_less_than_min */
            return false;

        if ( comp.compare(this.value, max) > 0 ) /* is_more_than_max */
            return false;
            
        if (this.left != null && !this.left.is_bst(min, this.value, comp))
            return false;

        if (this.right != null && !this.right.is_bst(this.value, max, comp))
            return false;

        return true;

    }

    public static <E> Tree<E> from_array(E[] array) {
        if (array == null ) return null;
        return from_array_rec(array, 0, array.length - 1);
    }

    public static <E> Tree<E> from_array_rec(E[] array, int from, int to) {
        if (from > to) return null;
        if (from == to) return new Tree<E>(array[from]);

        int pivot = from + (to-from+1)/2;

        Tree<E> current_root  = new Tree<E>(array[pivot]);
        Tree<E> left_c = from_array_rec(array, from, pivot-1);
        Tree<E> right_c = from_array_rec(array, pivot+1, to);

        if (left_c != null) left_c.parent = current_root;
        if (right_c != null) right_c.parent = current_root;

        current_root.left = left_c;
        current_root.right = right_c;

        return current_root;
    }

    public Tree<E> next() {

        if (this.right == null) {
            Tree<E> current = this;
            while ( current.parent != null && current == current.parent.right) {
                current = current.parent;
            }
            return current.parent;
        }

        Tree<E> next = this.right; 
        while (next.left != null) next = next.left;

        return next;
    }

    public void path_sum(int total) {
        path_sum(total, new LinkedList<E>(), new ArrayList<Integer>());
    }

    public void path_sum(int total, LinkedList<E> stack, ArrayList<Integer> partial_sums ) {
        stack.push(this.value);
        partial_sums.add(0,0);

        int val = 0;
        try {
            val = (Integer) this.value;
        } catch (ClassCastException orz) {} catch (NullPointerException orz) {}

        for (int i = 0; i != partial_sums.size(); ++i) {
            int new_partial = partial_sums.get(i) + val;
            partial_sums.set(i, new_partial);
            if (new_partial == total)
                stdout.println(look_into_stack(stack,i+1) + " = " + total);
        }

        if (this.left != null) this.left.path_sum(total, stack, partial_sums);
        if (this.right != null) this.right.path_sum(total, stack, partial_sums);

        for (int i = 0; i != partial_sums.size(); ++i) {
            int old_partial = partial_sums.get(i) - val;
            partial_sums.set(i, old_partial);
        }

        partial_sums.remove(0);
        stack.pop();
    }

    public static <E> String look_into_stack(LinkedList<E> stack, int depth) {
        StringBuffer string = new StringBuffer();
        LinkedList<E> unstacked = new LinkedList<E>();
        for(int i = 0; i != depth; ++i) {
            E node = stack.pop();
            string.append(node.toString());
            string.append(" + ");
            unstacked.push(node);
        }
        while(unstacked.isEmpty() == false) stack.push(unstacked.pop());
        return string.substring(0, string.length()-3);
    }

    public static void main(String argv[]) {

        int size_t = 11;

        if (argv.length > 0)
            try { size_t = Integer.parseInt(argv[0]); }
            catch (NumberFormatException orz) {}

        int total = 6;
        if (argv.length > 1)
            try { total = Integer.parseInt(argv[1]); }
            catch (NumberFormatException orz) {}


        Integer[] array = new Integer[size_t];

        for(int i = 0; i != size_t; ++i) array[i] = i;
/*
        Random random = new Random();
        for(int i = 1; i != size_t; ++i) {
            int j = random.nextInt(size_t-i);
            int swap = array[j];
            array[j] = array[size_t-i];
            array[size_t-i] = swap;
        }
*/
/*
        for(int i = 0; i != size_t; ++i) stdout.printf("%d ", array[i]);
        stdout.println();
*/
        Tree<Integer> tree = from_array(array);
/*
        Tree<Integer> node = tree.first();
        while(node != null) {
            node = node.next();
        }
*/

        for(Tree<Integer> node = tree.first(); node != null; node = node.next())
            stdout.printf("%d ", node.value);
        stdout.println();

        List<List<Tree<Integer>>> nodes = tree.nodes_hierarchy();
        
        for ( List<Tree<Integer>> rank : nodes ) {
            StringBuffer line = new StringBuffer();
            for (Tree<Integer> node : rank) {
                line.append(node.toString());
                line.append(" ");
            }
            stdout.println(line.toString());
        }

        boolean is_bst = tree.is_bst(new Comparator<Integer>(){
            public int compare(Integer x, Integer y) { return x - y;}
        });

        stdout.println(is_bst);

        tree.path_sum(total);

        System.exit(0);
    }

}
