import os

import psutil
import serial
from time import sleep
from threading import Thread

if os.name == "posix":
    import sensors

if os.name == "nt":
    PORT = "COM4"
elif os.name == "posix":
    PORT = "/dev/ttyACM0"
    os.nice(0)


class Monitor():
    def __init__(self):
        """Initialize"""
        print("Initialize")

        if os.name == "posix":
            sensors.init()

        self.ser = serial.Serial()
        self.ser.baudrate = 9600
        self.ser.port = PORT

        try:
            self.ser.open()
        except:
            print("no serial port")

        if os.name == "posix":
            self.chips = [[0 for feature in chip] for chip in sensors.iter_detected_chips()]

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
            cpu = psutil.cpu_percent(percpu=True)
            for i in range(self.cores):
                cpu[i] = str(cpu[i])
                if len(cpu[i]) == 3:
                    cpu[i] = cpu[i] + "0 "
                elif len(cpu[i]) == 4:
                    cpu[i] = cpu[i] + " "
                self.cpu[i] = cpu[i]

            self.mem = psutil.virtual_memory()
            self.disk = psutil.disk_usage('/')

            self.netio = psutil.net_io_counters()

            self.netup = self.netio.bytes_sent - self.lastnetup
            self.netdw = self.netio.bytes_recv - self.lastnetdw

            self.lastnetup = self.netio.bytes_sent
            self.lastnetdw = self.netio.bytes_recv

            self.netup = str(self.netup / 1000)
            self.netdw = str(self.netdw / 1000)

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


            # i = 0
            # for p in psutil.pids():
        #	proc = psutil.Process(p)
        #	print p , proc.name(), proc.cpu_percent()
        #	i += 1

    def send(self):
        while True:
            """Send Data"""
            sleep(1)

            # print"Cores =",self.cores
            self.ser.write("CORE {0} \0\n".format(str(self.cores)).encode("UTF-8"))
            sleep(0.1)

            # print"1.5v = ",self.chips[0][0]
            self.ser.write("1V {0} \0\n".format(str(self.chips[0][0])))
            sleep(0.1)

            # print"3.3v = ",self.chips[0][1]
            self.ser.write("3V {0} \0\n".format(str(self.chips[0][1])))
            sleep(0.1)

            # print"5.0v = ",self.chips[0][2]
            self.ser.write("5V {0} \0\n".format(str(self.chips[0][2])))
            sleep(0.1)

            # print"12.0v = ",self.chips[0][3]
            self.ser.write("12V {0} \0\n".format(str(self.chips[0][3])))
            sleep(0.1)

            # print"CPUFAN = ",self.chips[0][4]
            self.ser.write("CPUFAN {0} \0\n".format(str(self.chips[0][4])))
            sleep(0.1)

            # print"CPUTEMP = ",self.chips[0][5]
            self.ser.write("CPUTEMP {0} \0\n".format(str(self.chips[0][5])))
            sleep(0.1)

            # print"MBTEMP = ",self.chips[0][6]
            self.ser.write("MBTEMP {0} \0\n".format(str(self.chips[0][6])))
            sleep(0.1)

            # print"GPUTEMP = ",self.chips[1][0]
            self.ser.write("GPUTEMP {0} \0\n".format(str(self.chips[1][0])))
            sleep(0.1)

            # print"CPU =",self.cpuTotal
            self.ser.write("CPUTOTAL {0} \0\n".format(str(self.cpuTotal)))
            sleep(0.1)

            for i in range(self.cores):
                # print"C"+str(i+1)+" = "+str(self.cpu[i])
                self.ser.write("CPU {0} = {1} \0\n".format(str(i + 1), str(self.cpu[i])))
                sleep(0.1)

            # print "MEMPERC = "+str(self.mem.percent)
            self.ser.write("MEMPERC {0} \0\n".format(str(self.mem.percent)))
            sleep(0.1)

            # print "MEMTOTAL = "+str(self.mem.total/1000000)
            self.ser.write("MEMTOTAL {0} \0\n".format(str(self.mem.total / 1000000)))
            sleep(0.1)

            # print "MEMAVAIL = "+str(self.mem.available/1000000)
            self.ser.write("MEMAVAIL {0} \0\n".format(str(self.mem.available / 1000000)))
            sleep(0.1)

            # print "DISK = "+str(self.disk.percent)
            self.ser.write("DISK {0} \0\n".format(str(self.disk.percent)))
            sleep(0.1)

            # print "NET UP = "+str(self.netup/1000)
            self.ser.write("NETUP {0} \0\n".format(str(self.netup)))
            sleep(0.1)

            # print "NET DOWN = "+str(self.netdw/1000)
            self.ser.write("NETDW {0} \0\n".format(str(self.netdw)))
            sleep(0.1)


def main():
    monitor = Monitor()

    updateThread = Thread(target=monitor.update)
    sendThread = Thread(target=monitor.send)

    updateThread.daemon = True
    sendThread.daemon = True

    updateThread.start()
    sendThread.start()

    while (1):
        sleep(1000)

    sensors.cleanup()


if __name__ == '__main__':
    main()
