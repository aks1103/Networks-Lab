# Not working
import numpy as np 
import collections
import matplotlib.pyplot as plt
arr1 = []

with open("process_n.txt") as f:
    for line in f:
    	c = line.split()
    	print(c)
    	arr1.append(float(c[1]))

arr1 = np.array(arr)
mean = np.mean(arr)
std = np.std(arr)
print(mean)
print(std)

for i in range(len(arr)):
	arr1[i] = ((((arr1[i] - mean)/std)*100)//1)/100;


arr2 = []

with open("process_f.txt") as f:
    for line in f:
    	c = line.split()
    	print(c)
    	arr2.append(float(c[1]))

arr2 = np.array(arr)
mean = np.mean(arr)
std = np.std(arr)
print(mean)
print(std)

for i in range(len(arr)):
	arr2[i] = ((((arr2[i] - mean)/std)*100)//1)/100;

print((collections.Counter(arr)))

for i in range(len(arr)):
	arr[i] = ((((arr[i] - mean)/std)*100)//1)/100;


counter = dict(collections.Counter(arr))
l = list(counter.keys());
l.sort()		


for key in l:
	print(key)
	x.append(key)
	y.append(counter[key])

plt.scatter(x,y)
plt.show()
# print(arr.value_counts())
