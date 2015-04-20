from tes import waveform


x = waveform.time( sampling = 0.1, samples = 100)
p = waveform.freq( sampling = 0.1, samples = 100)

x2p = waveform.time2freq(x)
p2x = waveform.freq2time(p)

"""
print(x)
print(p2x)
print("")
print(p)
print(x2p)
"""

print( x == p2x)
print("")
print( p == x2p)
