package streams;

import java.lang.Iterable;
import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
/*
import streams.Function;
import streams.Operator;
import streams.Predicate;
*/
import streams.Stream;

public abstract class AbstractStream<E> implements Stream<E> {

    /* clients implement this */
    abstract public Iterator<E> iterator();

    public <F> Stream<F> map(Function<? extends F,? super E> transform) {
        return null;
    }

    public Stream<E> select(Predicate<? super E> check) {
        return this;
    }

    /* shouldn t first param of Operator be ? extend E ? */
    public E reduce(Operator<? extends E, ? super E> reducer) {
        /*
            get init_state from iterator
            call fold to leverage
        */
        return null;
    }

    // check food type of init_state
    public <F> F fold(Operator<? extends F, ? super E> folder, F init_state) {
        return null;
        /*
            prepare map operator which apply folder and return init_state
            define stream with this map operator
            return last state with last
        */
    }

    public E last() {
        return this.iterator().next();
    }

    public List<E> take(int n){ // return iterable<E> ?
        return new LinkedList<E>();
    }

    public Stream<E> buffer() {
        return this;
    }

}
