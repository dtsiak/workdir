#!/usr/bin/env python

import sys
import os
import re
import argparse
import itertools
import operator
#import numpy
#import scipy.spatial as scisp
import subprocess
#import matplotlib.pyplot as plt
#sys.path.append('/home/dtsiakoulias/.local/bin')

#sys.path.append('/home/dtsiakoulias/.local/lib/python3.6/site-packages')
#sys.path.append('/export/local/python-3.6.4/lib/python3.6/site-packages')
sys.path.append('/home/dtsiakoulias/.local/bin')
sys.path.append('/usr/local/python-2.7/lib/python2.7/site-packages')
#sys.path.append('/home/dtsiakoulias/PyCFTbootPARALLEL/pycftboot')
sys.path.append('/home/dtsiakoulias/.local/lib/python2.7/site-packages')
sys.path.append('/export/local/python-2.7/lib/python2.7/site-packages')

# Required argument is the directory
import scipy.spatial as scisp
import numpy



#new_points=[[0.518, 1.415, 1.045], [0.518, 1.42, 1.045], [0.518, 1.43, 1.055], [0.518, 1.435, 1.055], [0.518, 1.425, 1.045]]

#[[0.517, 1.42, 1.05], [0.517, 1.4, 1.04], [0.518, 1.42, 1.05], [0.518,   1.43, 1.05], [0.518, 1.41, 1.04], [0.517, 1.46, 1.08], [0.517,   1.48, 1.09], [0.517, 1.47, 1.09], [0.518, 1.48, 1.08], [0.518, 1.46,   1.07], [0.517, 1.44, 1.07], [0.517, 1.45, 1.07], [0.518, 1.45,   1.07], [0.518, 1.45, 1.06], [0.517, 1.5, 1.13], [0.518, 1.5,   1.12], [0.518, 1.5, 1.1], [0.517, 1.49, 1.1], [0.519, 1.5,   1.1], [0.518, 1.49, 1.09], [0.518, 1.49, 1.11], [0.518, 1.51,   1.11], [0.518, 1.49, 1.1], [0.519, 1.49, 1.09], [0.518, 1.5,   1.11], [0.517, 1.5, 1.12], [0.518, 1.52, 1.14], [0.518, 1.52,   1.13], [0.519, 1.55, 1.15], [0.518, 1.52, 1.12], [0.519, 1.52,   1.13], [0.52, 1.55, 1.14], [0.519, 1.55, 1.16], [0.519, 1.55,   1.13], [0.52, 1.55, 1.15], [0.519, 1.52, 1.11], [0.518, 1.53,   1.12], [0.518, 1.53, 1.14], [0.519, 1.53, 1.13], [0.518, 1.53,   1.13], [0.517, 1.53, 1.14], [0.517, 1.51, 1.12], [0.519, 1.51,   1.11], [0.519, 1.53, 1.12], [0.519, 1.51, 1.1], [0.518, 1.51,   1.13], [0.517, 1.51, 1.13], [0.518, 1.55, 1.15], [0.519, 1.55,   1.14], [0.518, 1.55, 1.16], [0.519, 1.56, 1.17], [0.519, 1.54,   1.13], [0.52, 1.57, 1.17], [0.519, 1.54, 1.14], [0.518, 1.56,   1.16], [0.519, 1.57, 1.17], [0.519, 1.56, 1.14], [0.518, 1.54,   1.14], [0.518, 1.54, 1.16], [0.518, 1.54, 1.13], [0.52, 1.56,   1.16], [0.52, 1.58, 1.18], [0.519, 1.59, 1.18], [0.52, 1.58,   1.16], [0.519, 1.58, 1.17], [0.52, 1.57, 1.15], [0.518, 1.57,   1.17], [0.52, 1.57, 1.16], [0.519, 1.57, 1.15], [0.519, 1.56, 1.16]]


#new_points=[[0.52, 1.58, 1.17], [0.52, 1.58, 1.16], [0.52, 1.58, 1.18], [0.519,  1.58, 1.17], [0.521, 1.58, 1.17], [0.519, 1.58, 1.18], [0.522, 1.58,  1.17], [0.522, 1.58, 1.16], [0.521, 1.58, 1.18], [0.522, 1.58,   1.18], [0.522, 1.58, 1.19], [0.523, 1.58, 1.17], [0.523, 1.585,   1.17], [0.52, 1.585, 1.17], [0.52, 1.585, 1.18], [0.521, 1.585,   1.16], [0.521, 1.585, 1.17], [0.521, 1.585, 1.18], [0.521, 1.585,   1.19], [0.519, 1.585, 1.17], [0.519, 1.585, 1.18], [0.522, 1.585,   1.17], [0.522, 1.585, 1.16], [0.522, 1.585, 1.18], [0.522, 1.585,   1.19]]

#new_points=[[0.519, 1.49], [0.519, 1.48], [0.52, 1.47], [0.52, 1.48], [0.521,   1.48], [0.52, 1.49], [0.52, 1.51], [0.521, 1.49], [0.521,   1.47], [0.52, 1.5], [0.521, 1.5], [0.521, 1.51], [0.522,   1.47], [0.522, 1.5], [0.522, 1.49], [0.522, 1.48], [0.522,   1.51], [0.524, 1.47], [0.524, 1.48], [0.523, 1.46], [0.523,   1.47], [0.523, 1.48], [0.523, 1.5], [0.523, 1.49], [0.524,   1.46], [0.514, 1.44], [0.514, 1.43], [0.514, 1.45], [0.514,   1.46], [0.514, 1.47], [0.515, 1.45], [0.515, 1.46], [0.515,   1.47], [0.515, 1.44], [0.513, 1.44], [0.513, 1.42], [0.513,   1.43], [0.513, 1.45], [0.515, 1.48], [0.517, 1.49], [0.516,   1.47], [0.516, 1.46], [0.516, 1.48], [0.516, 1.49], [0.517,   1.48], [0.517, 1.47], [0.518, 1.49], [0.519, 1.5]]


#for a in range(0, len(new_points)):
#   subprocess.call(["./subtestScratch.sh", str(new_points[a][0]), str(new_points[a][1]), str(new_points[a][2])])
#dphi_list=(0.518, 0.519, 0.52, 0.521)
#dphi=0.518 0.519
#dphi_list=(0.517, 0.518, 0.519, 0.52, 0.521, 0.522)
dphi=0.517
#D0_list=(1.41, 1.42, 1.43, 1.44, 1.45, 1.46, 1.47, 1.48, 1.49, 1.5, 1.51, 1.52, 1.53, 1.54, 1.55, 1.56, 1.57, 1.58)
#D0_list=(1.55, 1.56, 1.57, 1.58, 1.59, 1.6)
#D0_list=(1.45, 1.61, 1.62, 1.63)
#D0_list=(1.5, 1.51, 1.52, 1.53, 1.54, 1.55, 1.56, 1.57, 1.58, 1.59, 1.6)
D0=1.5
#D0_list=(1.41, 1.42, 1.43, 1.44, 1.45, 1.46, 1.47, 1.48, 1.49)
#for dphi in dphi_list:
# for D0 in D0_list:
#   x=1.04+0.875*(-1.43 + D0)
x=1.2
subprocess.call(["./subtestScratch.sh", str(dphi), str(D0), str(x)])
#   x1=x-0.01
#   x2=x-0.02
#   x3=x+0.01
#   x4=x+0.02
#   subprocess.call(["./subtestScratch.sh", str(dphi), str(D0), str(x1)])
#   subprocess.call(["./subtestScratch.sh", str(dphi), str(D0), str(x2)])
#   subprocess.call(["./subtestScratch.sh", str(dphi), str(D0), str(x3)])
#   subprocess.call(["./subtestScratch.sh", str(dphi), str(D0), str(x4)])









