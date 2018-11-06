lpt = ['Dummy',	"System up!",
	"Press any key to continue...", #input
	"Setup complete.",
	"Arrived at ", #has additional data that will have an underscore. eofStepperProg
	"Entering calibration mode. Enter 2 to move towards switch 1, enter 1 to move towards switch 2 to obtain location calibration: ", #accepts input
	"Towards Limit Switch 2: ",
	"Towards Limit Switch 1: ",
	"Hit switch 1",
	"Hit switch 2",
	"In calibration: Current location: ", #also has additional data after message. eof calibration_h
	"Current location: ", #has data
	"Options: (h)ome, (s)et destination, (p)reset motor speed, (c)alibration, (d)elay, (q)uit: ", #accepts input
	"You selected: ", #prints selection character
	"Going home...",
	"Current location: ", # has data
	"Moving towards Limit Switch 1", 
	"Moving towards Limit Switch 2",
	"Enter y to set delay, n to turn off delay.\n", #input char
	"The delay is in integer (ms) < 2000 ms, in 4 digits.\n", #input max 4 digits
	"Provide Destination (6 Digits): ", #input 6 digits
	"You have chosen: ", #data
	"Too close to either limits. Please enter new destinations. You can not go more than 2000 steps closer to the limits.", #message
	"Moving towards Limit Switch 2", #message
	"Moving towards Limit Switch 1", #message
	"Set RPM (3 digits): ", #input
	"RPM: ", #data
	"Quitting...", #message
	"Updated current location in EEPROM.",
	"Released motor.",
	"It is now safe to unplug the Arduino.",
	"Changing dir...", #message
	"Invalid number, try again.", #message. eof interaction_h
	"Hit limit switch 1 on irPin 1", #message
	"Enter number of steps away from Limit Switch 1 (must be between 1000 and 10000 (6 digits): ", #input
	"Hit limit switch 2 on irPin 2", #message
	"Enter number of steps away from Limit Switch 2 (must be between 1000 and 10000 (6 digits): ", #input
	"Hit stop switch on irPin 3", #message
	"Displacement: ", #data. eof irq.h
]
import sys
from time import sleep
import serial
if len(sys.argv) < 2:
	print ("Usage: specRead.py <device name in /dev/>")
	sys.exit(0)
ser = serial.Serial('/dev/'+sys.argv[1], 9600)
while True:
	line = ((ser.readline().rstrip()).decode('utf-8')) # Read the newest output from the Arduino
	words = line.split('_')
	mid = 0
	try:
		mid = int(words[0])
	except ValueError:
		#print(line)
		#print("Error on received line: ", line)
		continue
	if mid not in [0,2,5,12,18,19,20,25,34,36]:
		print(lpt[mid]+" ",end=' ')
		if len(words)>1:
			print(words[1:],end=' ')
		print('\n')
		if mid == 30:
			sleep(0.1)
			sys.exit(0)	
	
	if mid == 2 : #Press any key to continue
		a = input(lpt[2])
		ser.write(str.encode(a))
	
	if mid == 5 : #Entering calibration mode. Enter 2 to move towards switch 1, enter 1 to move towards switch 2 to obtain location calibration:
		while True:
			a = input(lpt[5])
			try:
				a = int(a)
			except ValueError:
				continue
			if a < 1 or a > 2 :
				continue
			else:
				break
		a = str(a).encode()
		ser.write(a)

	if mid == 12 : #Options: (h)ome, (s)et destination, (p)reset motor speed, (c)alibration, (d)elay, (q)uit
		while True:
			a = input(lpt[12])
			if len(a) != 1 :
				continue
			if a not in ['h','s','p','c','d','q']:
				continue
			break
		ser.write(a.encode())

	if mid == 18 :	#Enter y to set delay, n to turn off delay.
		while True:
			a = input(lpt[18])
			if len(a) != 1 :
				continue
			if a not in ['y','n']:
				continue
			break
		ser.write(a.encode())

	if mid == 19 : #The delay is in integer (ms) < 2000 ms, in 4 digits.
		while True:
			a = input(lpt[19])
			try:
				a = int(a)
			except ValueError:
				continue

			if a <= 2000:
				break
		a = format(a,'04d')
		a = str(a).encode()
		ser.write(a)

	if mid == 20 : #Provide Destination (6 Digits): 
		while True:
			a = input(lpt[20])
			print(a)
			try:
				a = int(a)
			except ValueError:
				continue

			if (a > 2000) and (a < (403840 - 6000)) :
				break
			else:
				continue
		a = format(a,'06d')
		print(a)
		a = str(a).encode()
		print(a)
		ser.write(a)

	if mid == 25: #Set RPM (3 digits): 
		while True:
			a = input(lpt[25])
			try:
				a = int(a)
			except ValueError:
				continue

			if a > 0 and a < 110 :
				break
		a = format(a,'03d')
		a = str(a).encode()
		ser.write(a)

	if mid == 34 : #Enter number of steps away from Limit Switch 1 (must be between 1000 and 10000 (6 digits): 
		while True:
			a = input(lpt[34])
			try:
				a = int(a)
			except ValueError:
				continue

			if a > 999 and a < 10001 :
				break
		a = format(a,'06d')
		a = str(a).encode()
		ser.write(a)

	if mid == 36 : #Enter number of steps away from Limit Switch 2 (must be between 1000 and 10000 (6 digits): 
		while True:
			a = input(lpt[34])
			try:
				a = int(a)
			except ValueError:
				continue

			if a > 999 and a < 10001 :
				break
		a = format(a,'06d')
		a = str(a).encode()
		ser.write(a)
