
from gi.repository import Gtk

import os
import sys
import serial
import psutil

if os.name == "posix": import sensors

from time import sleep
from threading import Thread


if os.name == "nt":
	PORT="COM21"
elif os.name == "posix":
	PORT="/dev/ttyACM0"
	os.nice(0)


class Monitor():
	def __init__(self):
		"""Initialize"""
		print("Initialize")
		sensors.init()
		self.ser = serial.Serial() 
		self.ser.baudrate = 9600
		self.ser.port = PORT
		#try:
		self.ser.open()
		#except:
		#	print"no serial port"

		self.chips = [[0 for feature in chip]for chip in sensors.iter_detected_chips()]
				
		self.cores = psutil.cpu_count()
		self.cpuTotal = psutil.cpu_percent()
		self.cpu = [0 for cores in range(self.cores)]
		self.mem = psutil.virtual_memory()
		self.disk = psutil.disk_usage('/')
		self.netio = psutil.net_io_counters()
		self.netup = 0
		self.netdw = 0
		self.lastnetup = 0
		self.lastnetdw = 0
		self.interval = 1
	
	def update(self):
		while True:
			"""Update Data"""
			if os.name == "posix":
				i = 0
				for chip in sensors.iter_detected_chips():
					j = 0
					for feature in chip:
						self.chips[i][j] = feature.get_value() 
						j += 1
					i += 1

			cpuTotal = psutil.cpu_percent()
			self.cpuTotal = str(cpuTotal)
			cpu = psutil.cpu_percent(percpu = True)
			for i in range(self.cores):
				cpu[i] = str(cpu[i])
				if len(cpu[i]) == 3 :
					cpu[i] = cpu[i]+"0 "
				elif len(cpu[i]) == 4 :
					cpu[i] = cpu[i]+" "
				self.cpu[i] = cpu[i]
		
			self.mem = psutil.virtual_memory()
			self.disk = psutil.disk_usage('/')
			
			self.netio = psutil.net_io_counters()
			
			self.netup = self.netio.bytes_sent - self.lastnetup
			self.netdw = self.netio.bytes_recv - self.lastnetdw
			
			self.lastnetup = self.netio.bytes_sent
			self.lastnetdw = self.netio.bytes_recv
			
			self.netup = str(self.netup/1000)
			self.netdw = str(self.netdw/1000)
						
			if len(self.netup) == 1:
				self.netup = self.netup + "  "
			if len(self.netup) == 2:
				self.netup = self.netup + " "
			
			if len(self.netdw) == 1:
				self.netdw = self.netdw + "  "
			if len(self.netdw) == 2:
				self.netdw = self.netdw + " "
			if len(self.netdw) == 3:
				self.netdw = self.netdw + " "
			
			sleep(1)
		
		
		#i = 0
		#for p in psutil.pids():
		#	proc = psutil.Process(p)
		#	print p , proc.name(), proc.cpu_percent()
		#	i += 1
		
	
	def send(self):
		while True:
			"""Send Data"""
			sleep(1)
			
			
			
			#print"Cores =",self.cores
			self.ser.write("CORE"+str(self.cores)+"\0\n")
			sleep(0.1)

			#print"1.5v = ",self.chips[0][0]
			self.ser.write("1V"+str(self.chips[0][0])+"\0\n")
			sleep(0.1)
			
			#print"3.3v = ",self.chips[0][1]
			self.ser.write("3V"+str(self.chips[0][1])+"\0\n")
			sleep(0.1)
			
			#print"5.0v = ",self.chips[0][2]
			self.ser.write("5V"+str(self.chips[0][2])+"\0\n")
			sleep(0.1)
			
			#print"12.0v = ",self.chips[0][3]
			self.ser.write("12V"+str(self.chips[0][3])+"\0\n")
			sleep(0.1)
			
			#print"CPUFAN = ",self.chips[0][4]
			self.ser.write("CPUFAN"+str(self.chips[0][4])+"\0\n")
			sleep(0.1)
			
			#print"CPUTEMP = ",self.chips[0][5]
			self.ser.write("CPUTEMP"+str(self.chips[0][5])+"\0\n")
			sleep(0.1)
			
			#print"MBTEMP = ",self.chips[0][6]
			self.ser.write("MBTEMP"+str(self.chips[0][6])+"\0\n")
			sleep(0.1)
			
			#print"GPUTEMP = ",self.chips[1][0]
			self.ser.write("GPUTEMP"+str(self.chips[1][0])+"\0\n")
			sleep(0.1)

			#print"CPU =",self.cpuTotal
			self.ser.write("CPUTOTAL"+str(self.cpuTotal)+"\0\n")
			sleep(0.1)
	
			for i in range(self.cores):
				#print"C"+str(i+1)+" = "+str(self.cpu[i])
				self.ser.write("CPU"+str(i+1)+"="+str(self.cpu[i])+"\0\n")
				sleep(0.1)
	
			#print "MEMPERC = "+str(self.mem.percent)
			self.ser.write("MEMPERC"+str(self.mem.percent)+"\0\n")
			sleep(0.1)

			#print "MEMTOTAL = "+str(self.mem.total/1000000)
			self.ser.write("MEMTOTAL"+str(self.mem.total/1000000)+"\0\n")
			sleep(0.1)

			#print "MEMAVAIL = "+str(self.mem.available/1000000)
			self.ser.write("MEMAVAIL"+str(self.mem.available/1000000)+"\0\n")
			sleep(0.1)
			
			#print "DISK = "+str(self.disk.percent)
			self.ser.write("DISK"+str(self.disk.percent)+"\0\n")
			sleep(0.1)
			
			#print "NET UP = "+str(self.netup/1000)
			self.ser.write("NETUP"+str(self.netup)+"\0\n")
			sleep(0.1)
			
			#print "NET DOWN = "+str(self.netdw/1000)
			self.ser.write("NETDW"+str(self.netdw)+"\0\n")
			sleep(0.1)

def main():

	monitor = Monitor()

	updateThread = Thread(target=monitor.update)
	sendThread = Thread(target=monitor.send)

	updateThread.daemon = True	
	sendThread.daemon = True
	
	updateThread.start()
	sendThread.start()
	
	while(1):
		sleep(1000)

	sensors.cleanup()

if __name__ == '__main__':
    main()
