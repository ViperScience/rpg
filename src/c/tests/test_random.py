# Plot the output from the krand test app to see if it is uniformly distributed.

import numpy as np
import matplotlib.pyplot as plt

# Read the data output from test_krand
data = np.loadtxt("randomoutput.txt")

# Draw histogram
n, bins, patches = plt.hist(data, 50, normed=True, facecolor='g', alpha=0.75)
plt.xlabel('RNG Value')
plt.ylabel('Probability')
plt.title('Histogram of Park & Miller RNG')
plt.text(.025, .035, r'$N = {},\ \mu={}$'.format(len(data), np.mean(data)))
plt.axis([0, 1, 0, 1.2])
plt.grid(True)
plt.show()
