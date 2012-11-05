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
import tes.filter                   # for moving average


def byte1():
	return numpy.linspace( 0, 0, 256 )


def add(histo, where):
	try:
		histo[128+where] += 1
	except IndexError:
		sys.stderr.write( "tes.histogram.add index err @ 128 + %s\n" % str(where))


def smooth(histo, smoothing):
	return tes.filter.moving_average(histo, smoothing)[0:len(histo)]


def find_peaks(histo):
	"""
	return position of peaks in histo
	"""
	return []


def visibility(histo, fringes = 3):
	"""
	looks like it works lol
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
		#print( n_min_at, n_min, "vis", contrast( n_min, n_max ) )
		#print( p_min_at, p_min, "vis", contrast( p_min, p_max ) )
		mean_vis += contrast( n_min, n_max )
		mean_vis += contrast( p_min, p_max )
		# find next maximum to get one fringe
		n_max_at, n_max = next_max(histo, n_min_at)
		p_max_at, p_max = prev_max(histo, p_min_at)
		#print( n_max_at, n_max, "vis", contrast( n_min, n_max ) )
		#print( p_max_at, p_max, "vis", contrast( p_min, p_max ) )
		mean_vis += contrast( n_min, n_max )
		mean_vis += contrast( p_min, p_max )
		mean_num += 4
	return mean_vis / mean_num


def next_min(histo, where):
	local_min = histo[where]
	for next in range(where, len(histo)):
		if histo[next] > local_min: break  # break 1-step after min
		local_min = histo[next] 
	return next-1, local_min


def next_max(histo, where):
	local_max = histo[where]
	for next in range(where, len(histo)):
		if histo[next] < local_max: break  # break 1-step after max
		local_max = histo[next] 
	return next-1, local_max


def prev_min(histo, where):
	local_min = histo[where]
	for prev in reversed( range(0, where) ):
		if histo[prev] > local_min: break  # break 1-step before min
		local_min = histo[prev] 
	return prev+1, local_min


def prev_max(histo, where):
	local_max = histo[where]
	for prev in reversed( range(0, where) ):
		if histo[prev] < local_max: break  # break 1-step before max
		local_max = histo[prev] 
	return prev+1, local_max


def contrast(a, b):
	return (max([a,b])-min([a,b]))/(a+b)


