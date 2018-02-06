import requests
import os

duration = 0.1  # second
freq = 440  # Hz

http_proxy  = "http://user:pass@proxyserver:proxyport"
https_proxy = "http://user:pass@proxyserver:proxyport"
ftp_proxy   = "http://user:pass@proxyserver:proxyport"

proxyDict = { 
              "http"  : http_proxy, 
              "https" : https_proxy, 
              "ftp"   : ftp_proxy
            }

url = "facebook.com"
sizes = [128, 256 , 512, 1024 , 2048 , 800]


for size in sizes:
	for i in range(3):
		f = open("size.data" , "a+")
		r = requests.get('http://www.spfld.com/cgi-bin/ping?remote_host='+ url + '&dns=on&count=10&size='+ str(size), proxies=proxyDict)
		os.system('play --no-show-progress --null --channels 1 synth %s sine %f' % (duration, freq))
		print(r.text)
		f.write(r.text)

duration = 1
os.system('play --no-show-progress --null --channels 1 synth %s sine %f' % (duration, freq))
f.close()

