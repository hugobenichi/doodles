import time

def fib(n):
    a, b = 1, 0
    while n > 0:
        c = a
        a = a + b
        b = c
        n -= 1
    return b

start = time.time()

count = 100
while (count > 0):
    x = fib(40)
    count -= 1;

stop = time.time()

print(x, 1000 * 1000 * (stop - start))
