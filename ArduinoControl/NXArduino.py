import serial as ser
import time

se=ser.Serial("/dev/ttyUSB0",115200,timeout=1)
time.sleep(1)

count=0	
while True:
	
	arduino=se.readline().decode()
	print(repr(arduino))
	print(type(arduino))
	print(len(arduino))
	print()
	if arduino =="start":#----------------初始化(移動到進料位)---------------
		se.write("feed_detection".encode())#---------初始化進料確認-------------
		count=1

	if arduino=="ok" and count==0:
		se.write("feed_detection".encode())#---------進料確認-------------
		count=1	
	
	elif arduino=="ok" and count==1:
		se.write("move2".encode())#------------移動到下方拍照位-----------
		count=2

	elif arduino=="ok" and count ==2:
		se.write("photo".encode())#------------拍照燈光--------------
		count=3
	elif arduino=="ok" and count ==3:
		se.write("move3".encode())#-------------移動到上方錄影位------------
		count=4

	elif arduino=="ok" and count ==4:
		se.write("video".encode())#------------錄影燈光--------------
		count=5

	elif arduino=="ok" and count ==5:
		se.write("defect".encode())#----------有缺陷出料(並移動到進料位)-----------
		count=6

	elif arduino=="ok" and count ==6:
		se.write("perfect".encode())#----------無缺陷出料(並移動到進料位)----------
		count=0

	
	
	time.sleep(1)
	
