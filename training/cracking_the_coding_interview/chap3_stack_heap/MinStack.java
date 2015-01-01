import java.util.*;


interface MinimumStack { // MinStack<E extends Comparable<? super E>MinStack.java
    public void push(Integer x);
    public Integer pop();
    public Integer min();
}


public class MinStack implements MinimumStack{ 

    private LinkedList<Integer> stack;
    private LinkedList<Integer> min;

    public MinStack() {
        this.stack = new LinkedList<Integer>();
        this.min = new LinkedList<Integer>();
    }

    public Integer min() {
        return this.min.peek();
    }

    public void push(Integer x) {
        this.stack.push(x);
        if ( x.compareTo(this.min.peek()) <= 0) this.min.push(x);
    }

    public Integer pop() {
        Integer x = this.stack.pop();
        if (x == this.min.peek()) this.min.pop();
        return x;
    }

    public static void insert_into(LinkedList<Integer> stack, Integer x ) {
        LinkedList<Integer> tail = new LinkedList<Integer>();
        
        while( stack.isEmpty() == false && stack.peek() < x ) {
            System.out.print(stack.peek().toString() + " ");
            tail.push( stack.pop());
        }
        System.out.println( "pushing " + x.toString());
        stack.push(x);
        while( tail.isEmpty() == false)
            stack.push(tail.pop());
    }

    public static void sort(LinkedList<Integer> stack) {
        LinkedList<Integer> head = new LinkedList<Integer>();

        while(stack.isEmpty() == false)
            insert_into(head, stack.pop());

        while(head.isEmpty() == false)
            stack.push(head.pop());
    }

    public static void print_list(LinkedList<Integer> stack) {
        StringBuffer string = new StringBuffer();
        for (Integer x : stack) {
            string.append(x.toString());
            string.append(" ");
        }
        System.out.println(string.toString());
    }

    public static void main(String argv[]) {
    
        int size = 10; 
        if (argv.length > 0 )
            try { size = Integer.parseInt(argv[0]); }
            catch (NumberFormatException orz) {}

        LinkedList<Integer> stack = new LinkedList<Integer>();
        int[] array = new int[size];
        Random rand = new Random();

        for (int i = 0; i != size; ++i) array[i] = i;
        for (int i = 1; i != size; ++i) {
            int j = rand.nextInt(size-i);
            int swap = array[j];
            array[j] = array[size-i];
            array[size-i] = swap;
            stack.push(swap);
        }
        stack.push(array[0]);

        print_list(stack);

        sort(stack);

        print_list(stack);

        System.exit(0);
    }

}

/*


[ head ] pivot [ tail ]

if pivot > tail.first
    insert tail.first in head

*/
