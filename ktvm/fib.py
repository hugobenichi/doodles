import time

def fib(n):
    a, b = 1, 0
    while n > 0:
        c = a
        a = a + b
        b = c
        n -= 1
    return b
start = time.monotonic()
count = 10
while (count > 0):
    x = fib(19)
    count -= 1;
stop = time.monotonic()
print(x, 1000 * 1000 * (stop - start))
