import java.util.*;


interface MyQueue<E> {
    public void push(E elem);
    public E pop();
    public boolean isEmpty();
}


public class StackQueue<E> implements MyQueue<E> {

    private enum QueueMode {INPUT, OUTPUT}

    private LinkedList<E> input;
    private LinkedList<E> output;
    private QueueMode mode;

    public StackQueue(){
        this.input = new LinkedList<E>();
        this.output = new LinkedList<E>();
        this.mode = mode.INPUT;
    }

    private void switch_input_mode() {
        while(output.isEmpty() == false)
            input.push(output.pop());
        this.mode = mode.INPUT;
    }

    private void switch_output_mode() {
        while(input.isEmpty() == false)
            output.push(input.pop());
        this.mode = mode.OUTPUT;
    }

    public void push(E elem){
        if ( this.mode == mode.OUTPUT ) this.switch_input_mode();
        input.push(elem);
    }

    public E pop() {
        if ( this.mode == mode.INPUT ) this.switch_output_mode();
        return output.pop();
    }

    public boolean isEmpty() {
        return input.isEmpty() && output.isEmpty();
    }

}
