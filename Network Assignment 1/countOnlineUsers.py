import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np


plt.ion() 
fig=plt.figure()
# plt.axis([])
plt.show()

def findOnlineUsers(ip,netmask):
	result = subprocess.run(['nmap', '-n' , '-sP', ip + "/" + str(netmask) ], stdout=subprocess.PIPE)
	output = result.stdout.decode('utf-8')
	users_online = output.count("Host is up")
	return users_online



if __name__ == "__main__":
	

	time_btw_pings = 40  # in secs
	start_time = time.time()
	ip = "10.3.1.11"
	netmask = 22
	time_entries = []
	users_online_entries = []
	# while True:
	for i in range(10):
		t = time.time()- start_time
		# t = t//100
		
		time_entries.append(t)
		users_online=findOnlineUsers(ip,netmask)
		users_online_entries.append(users_online)

		plt.scatter(t,users_online);
		plt.show()
		plt.pause(0.0001) #Note this correction

		print(t , users_online)
		time.sleep(time_btw_pings)

	print(time_entries)
	print(users_online_entries)

