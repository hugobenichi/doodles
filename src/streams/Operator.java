package streams;

public interface Operator<F,E> { F call(F left_operand, E right_operand); }
