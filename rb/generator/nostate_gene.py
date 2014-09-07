#!/usr/bin/env python3

null_stream = (None,None)

def int_from(n):    return (n, lambda: int_from(n+1))  	# lambda for delayed evaluation
def head( stream ): return stream[0]									
def tail( stream ): return stream[-1]()  
													
def take(n, stream):
  if n <= 0: return []
  return [ head(stream) ] + take(n-1, tail(stream) )	

def filt( test, stream ):
  if test( head(stream) ): return [head(stream)] + filt(pred, tail(stream) )
  else:                    return filt( test, tail(stream) )



def red(func, stream, carry):
  if stream is null_stream: return carry
  else:                     return red( func, tail(stream), func( carry, head(stream) ) )
    
def ltake(n, stream):
  if n <= 0: return null_stream
  else:      return ( head(stream), lambda: take(n-1, tail(stream) ) )

def lfilt( test, stream ):
	if test( head(stream) ): return (head(stream), lambda: lfilt(pred, tail(stream) ) )
	else: return lfilt( test, tail(stream) )
	
def to_array(stream):
	return red(lambda a, x: a + [x], stream, [])

def sieve(stream):
	h = head(stream)
	return ( head(stream), lambda: sieve( filter(lambda x: x%h != 0, tail(stream))  ) )

   
print( to_array( ltake(10, int_from(1) ) ) )


def rec_stream(f):
	"""
	can be applied to newton's method for approx roots
	  x_(n+1) = x_(n) - f[ x_(n) ] / f'[ x_(n) ]
	"""
	return (f, lambda: rec_stream( lambda x: f(f(x) ) ) )
	
def newton(f, fprime):
	return lambda x: x - f(x)/float( fdash(x) )
  
def newton_solver( iters, f, fdash ):
	def solve(val):
		n = newton( lambda x: f(x) - val, fdash )
		stream = rec_stream(n)
		return to_array( take(iters, stream) )
	return solve
