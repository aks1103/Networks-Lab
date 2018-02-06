from random import *
import matplotlib.pyplot as plt
import matplotlib.dates
from datetime import datetime
import time
import matplotlib.dates as mdates

lines = []

with open('lab.csv', 'r') as f:
    lines = f.readlines()

entries = []
times = []
values = []

for l in range(2,216,5):
    entry = lines[l].rstrip().split(',')
    datetime_object = datetime.strptime("0 0 0 " + entry[0], '0 0 0 %H:%M:%S')
    print (datetime_object)
    times.append(entry[0])
    entry[1] = int(entry[1])    
    entries.append(datetime_object)
    values.append(entry[1])

print(times)
print(values)

dates = matplotlib.dates.date2num(entries)
myFmt = mdates.DateFormatter('%H:%M')
plt.gca().xaxis.set_major_formatter(myFmt)
plt.xlabel('Time of Day', fontsize=16)
plt.ylabel('Online Users', fontsize=14)
plt.plot_date(dates, values , ls="dashed", color="g")
plt.show()
