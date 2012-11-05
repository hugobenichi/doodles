########################################################
#                                                      #
#   routines related to histogram manipulation         #
#                                                      #
#	  creation:    2012/11/05                            #   
#   based on:    github.com/hugobenichi/tes            #
#   copyright:   hugo benichi 2012                     #
#   contact:     hugo[dot]benichi[at]m4x[dot]org       #
#                                                      #
########################################################


import sys                          # for error output
import numpy                        # for numpy array


def byte1():
	"""
	creates a numpy array for counting 8 bits valued sequence.
	"""
	return numpy.linspace( 0, 0, 256 )


def add(histo, where):
	"""
	update a counting histogram made of 256 bins for a sequence
	of 8-bits signed char values.
	"""
	try:
		histo[128+where] += 1
	except IndexError:
		sys.stderr.write( "tes.histogram.add: index err @ 128 + %s\n" % str(where))

def visibility(histo, fringes = 3):
	"""
	(looks like it works lol)
	compute the average visibility of a fringes, starting from
	the max value. Nearest local minimums and local maximums
	are iteratively detected and used to calculate the contrast
	of single peaks. The visibility is the mean peak-bottom to
	peak-top contrast of a <fringes> number of peaks, where 
	<fringes> is a named argument (default value: 3).
	"""
	for lmax_at, lmax in enumerate(histo):
		if lmax == max(histo): break
	mean_vis, mean_num = 0, 0
	n_max_at, p_max_at = lmax_at, lmax_at
	n_max, p_max = lmax, lmax
	for num in range(0,int(fringes/2)):
		#find bottom of fringe first
		n_min_at, n_min = next_min(histo, n_max_at)
		p_min_at, p_min = prev_min(histo, p_max_at)
		mean_vis += contrast( n_min, n_max )
		mean_vis += contrast( p_min, p_max )
		# find next maximum to get one fringe
		n_max_at, n_max = next_max(histo, n_min_at)
		p_max_at, p_max = prev_max(histo, p_min_at)
		mean_vis += contrast( n_min, n_max )
		mean_vis += contrast( p_min, p_max )
		mean_num += 4
	return mean_vis / mean_num


def next_min(histo, where):
	"""
	locate the next local minimal value in an array starting
	from <where>.
	"""
	local_min = histo[where]
	for next in range(where, len(histo)):
		if histo[next] > local_min: break  # break 1-step after min
		local_min = histo[next] 
	return next-1, local_min


def next_max(histo, where):
	"""
	locate the next local maximal value in an array starting
	from <where>.
	"""
	local_max = histo[where]
	for next in range(where, len(histo)):
		if histo[next] < local_max: break  # break 1-step after max
		local_max = histo[next] 
	return next-1, local_max


def prev_min(histo, where):
	"""
	locate the previous local minimal value in an array going back
	from <where>.
	"""
	local_min = histo[where]
	for prev in reversed( range(0, where) ):
		if histo[prev] > local_min: break  # break 1-step before min
		local_min = histo[prev] 
	return prev+1, local_min


def prev_max(histo, where):
	"""
	locate the previous local maximal value in an array going back
	from <where>.
	"""
	local_max = histo[where]
	for prev in reversed( range(0, where) ):
		if histo[prev] < local_max: break  # break 1-step before max
		local_max = histo[prev] 
	return prev+1, local_max


def contrast(a, b):
	"""
	computes the contrast given the min and max value, given in any order.
	"""
	return (max([a,b])-min([a,b]))/(a+b)


