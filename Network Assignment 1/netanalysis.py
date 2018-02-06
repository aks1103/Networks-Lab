#  Local network analysis: Query your LAN using nmap to discover which hosts are online. Use a command such
# as: nmap –n –sP <Subnet Range>
#  (eg., 172.16.112.0/26)
# You can choose a different LAN subnet address as well (make sure you report the same in your report explicitly).
# Now run the command repeatedly at different times of the day, and finds the number of hosts online. Do it for at
# least 5 times with sufficient time gap. Plot a graph against time to see if there are any hourly trends to when
# computers are switched ON or OFF in your LAN.


import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import datetime
import os
duration = 1  # second
freq = 440  # Hz

proxy = set()

# Get Online Users
def findOnlineUsers(ip_range):
	result = subprocess.run(['nmap', '-n' , '-sP', ip_range ], stdout=subprocess.PIPE)
	output = result.stdout.decode('utf-8')

	online_users = []
	for line in output.split("\n"):
		isUp = line.find("Host is up")
		if isUp != -1:
			data = prev.split()
			print(data[len(data) - 1])
			online_users.append(data[len(data) - 1])
		prev = line

	return online_users


# Check Services running on User PC
def checkServices(ip):
	result = subprocess.run(['nmap' , ip  ], stdout=subprocess.PIPE)
	output = result.stdout.decode('utf-8')
	return output


def checkSquidProxy(ip):
	result = subprocess.run(['nmap' , ip  ], stdout=subprocess.PIPE)
	output = result.stdout.decode('utf-8')
	for line in output.split("\n"):
		if line.find("squid") != -1:
			print("Squid Proxy Server found at:")
			data = line.split()
			data = data[0]
			port = data.split("/")[0]
			print("IP : " + str(ip) + "\nPort :" + str(port))
			os.system('play --no-show-progress --null --channels 1 synth %s sine %f' % (duration, freq))

		if line.find("proxy") != -1:
			print("Proxy Server found at:")
			data = line.split()
			data = data[0]
			port = data.split("/")[0]
			print("IP : " + str(ip) + "\nPort :" + str(port))
			os.system('play --no-show-progress --null --channels 1 synth %s sine %f' % (duration, freq))
	return output


if __name__ == "__main__":
	start_time = time.time()
	ip_range = input("Enter IP Range :")
	online_users=findOnlineUsers(ip_range)

	for user in online_users:
		print("Analysing user " + str(user) + ":\n")
		print(checkServices(user))
