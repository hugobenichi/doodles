import java.util.*;


public class LList<E> {

    private Link<E> top, bottom;

    private class Link<F> {
        public Link<F> next;
        public F value;
        public Link(F item){ next = null; this.value = item;}
        public String toString() {return this.value.toString();}
        /*
        public Link<F> reverse(){ 
            // iterative version //
            Link<F> head = null, pivot = this, tail = this;
            while(tail != null) {
                pivot = tail;
                tail = tail.next;
                pivot.next = head;
                head = pivot;
            }
            return pivot;
        }
        */
        public Link<F> reverse(){ 
            // recursive version //
            if (this.next == null) return this;
            Link<F> tail = this.next;
            Link<F> head = tail.reverse();
            tail.next = this;
            this.next = null;
            return head;
        }
    }

    public LList() { this.top = null; this.bottom = null; }

    public void init(Link<E> link){ this.top = link; this.bottom = link; }

    public boolean isEmpty(){ return this.top == null ? true : false;}


    public void add(E item) {
        Link<E> new_link = new Link<E>(item);
        if (this.isEmpty()) { this.init(new_link); }
        else                { new_link.next = this.top; this.top = new_link; }
    }
    
    public void link(E item) {
        Link<E> new_link = new Link<E>(item);
        if (this.isEmpty()) { this.init(new_link); }
        else                { this.bottom = new_link; }
    }

//    public E    pop(){}

    public String toString(){
        StringBuffer string = new StringBuffer();
        Link<E> pivot = this.top;
        while ( pivot != null ) {
            string.append(pivot.toString());
            string.append(" ");
            pivot = pivot.next;
        }
        return string.toString();
    }

    public void reverse() { 
        Link<E> head = this.top;
        this.top = this.top.reverse();
        this.bottom = head;
    }

    public static void main(String argv[]) {

        LList<String> l = new LList<String>();
        l.add("foo");
        l.add("bar");
        l.add("tos");
        l.add("etron");
        l.add("bite");

        System.out.println(l);

        l.reverse();

        System.out.println(l);

        System.exit(0);

    }

}
