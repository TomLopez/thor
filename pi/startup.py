import requests
import time
import smbus

from subprocess import call

bus = smbus.SMBus(1)
# This is the address we setup in the Arduino Program
address = 0x04

print "getting data from arduino"
# Getting fill state of trash
bus.write_byte(address, 1)
time.sleep(1)
fillState = bus.read_byte(address)
print "fill state: "
print fillState
time.sleep(1)
bus.write_byte(address, 2)
time.sleep(1)
battery = bus.read_byte(address)
print "battery: "
print battery
time.sleep(1)
bus.write_byte(address, 3)
time.sleep(1)
bCrushed = bus.read_byte(address)
print "bottles crushed: "
print bCrushed
print "got data"

print "requesting"

loopVar = 0
while (loopVar==0):
	print "in while loop"
	print "start"
	try:
		url = "http://thor.fabianschulz.ch/fillBin.php?id=1&fill="+str(fillState)+"&battery="+str(battery)+"&bCrushed="+str(bCrushed)+"&pw=thorSmartBin"
		print url
		r = requests.get(url, timeout=1)
		print "end"
		if r.content=="done":
			print "good"
			call("sudo shutdown now", shell=True)
		else:
			print "not good"
			loopVar=1
	except requests.exceptions.Timeout:
   		# Maybe set up for a retry, or continue in a retry loop
		print "timeout, waiting"
		pass
	except requests.exceptions.TooManyRedirects:
    		# Tell the user their URL was bad and try a different one
		print "Too many redirects"
		pass
	except requests.exceptions.RequestException as e:
    		# catastrophic error. bail.
		print "catastrophic error"
    		print e
		pass
	print "waiting"
	time.sleep(2)

print "done"
