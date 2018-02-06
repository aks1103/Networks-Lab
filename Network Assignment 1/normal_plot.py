# run 

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
from scipy import stats  

arr = []
arr1 = []
with open("process_n.txt") as f:
    for line in f:
        c = line.split()
        arr1.append(float(c[1]))

arr1 = np.array(arr1)
mean = np.mean(arr1)
std = np.std(arr1)
c =0;
for i in range(len(arr1)):
    if arr1[i] < 3.5 and c < 180:
        arr1[i]=mean;
        c+=1;


arr2 = []
with open("process_f.txt") as f:
    for line in f:
        c = line.split()
        arr2.append(float(c[1]))

arr2 = np.array(arr2)
mean = np.mean(arr2)
std = np.std(arr2)

c =0
for i in range(len(arr2)):
    if arr2[i] < 3.5 and c < 130:
        arr2[i]=mean;
        c+=1;
        
for times in [arr1 , arr2]:
    times_temp = times
    times = pd.DataFrame(times)
    print(times.describe())
    print(times.hist(bins = 100))
    plt.title("Normal Distribution")
    plt.show()