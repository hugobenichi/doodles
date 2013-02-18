package streams;

import java.lang.Iterable;
import java.util.List;

import streams.Function;
import streams.Operator;
import streams.Predicate;

public interface Stream<E> extends Iterable<E> {

    <F> Stream<F>   map(Function<? extends F,? super E> transform);

        Stream<E>   select(Predicate<? super E> check);

        // shouldn t first param of Operator be ? extend E ?
        E           reduce(Operator<? extends E, ? super E> reducer);

    <F> F           fold(Operator<? extends F, ? super E> folder, F init_state);

        E           last();

        List<E>     take(int n);

        Stream<E>   buffer();

}
