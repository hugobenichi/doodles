

checkpoints = list()

def checkpoint_set():
    """create a new local checkpoint."""
    checkpoints.append(list())

def checkpoint_unset():
    """go back to where the checkpoint was set by unwinding the callstack."""
    unwind(checkpoints.pop())

def unwind(callstack):
    """pop from callstack in reverse order and apply undo func to args."""
    while callstack:
        (func, ret_val, args, kwargs) = callstack.pop()
        func(*args, **kwargs)

def callstack_now():
    """just return last checkpoint."""
    return checkpoints[-1]

def callstack_push(*frame):
    """push args and undo func to top of callstack. With return value."""
    callstack_now().append(frame)

def reverse(undo_function):
    """parametric decorator."""
    def enqueue(do_function):
        """decorate a 'do' function by saving its argument and return value
        on top of local callstack."""
        def tagged_do_function(*args, **kwargs):
            ret_val = do_function(*args, **kwargs)
            callstack_push(undo_function, ret_val, args, kwargs)
            return ret_val
        return tagged_do_function
    return enqueue

def checkpoint(function_to_rewind):
    """decorator for wrapping a test case with checkpoint."""
    def wrapped_func(*args, **kwargs):
        checkpoint_set()
        function_to_rewind(*args, **kwargs)
        checkpoint_unset()
    return wrapped_func


# some basic examples

def foo_undo(a):
    """function which can reverse side effects of another function"""
    print "undoing foo with arg", a

@reverse(foo_undo)
def foo_do(a):
    """define a function with side effect (let's imagine it has some), so you
    tag it with its associated 'reverse' function using decorator @reverse."""
    print "doing foo with arg", a

def bar_undo(a): print "undoing bar with arg", a

@reverse(bar_undo)
def bar_do(a): print "doing bar with arg", a

@checkpoint
def test_a():
    """that s a test which uses side-effect functions,
    so we tag it with @checkpoint."""
    foo_do(4)
    foo_do("hello")
    bar_do([1,2,3])

@checkpoint
def test_b():
    bar_do(42)
    bar_do(10)

@checkpoint
def test_c():
    """tests can be nested, yet the call 'tree' will be unwinded correctly."""
    foo_do("before")
    test_a()
    bar_do("after")

test_a()
test_b()
test_c()
