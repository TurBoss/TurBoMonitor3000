import os

import psutil
import serial
from time import sleep

import status_pb2

import sensors

PORT = "/dev/ttyACM0"
os.nice(0)


class Monitor:
    def __init__(self):
        """Initialize"""
        print("Initialize")

        self.metrics = status_pb2.CpuStatus()
        sensors.init()

        self.ser = serial.Serial()
        self.ser.baudrate = 9600
        self.ser.port = PORT

        try:
            self.ser.open()
        except Exception as e:
            print("no serial port {}".format(e))

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

    def __del__(self):
        sensors.cleanup()

    def update(self):
        """Update Data"""
        i = 0
        for chip in sensors.iter_detected_chips():
            j = 0
            for feature in chip:
                self.chips[i][j] = feature.get_value()
                j += 1
            i += 1

        self.cpuTotal = psutil.cpu_percent()
        self.cpu = psutil.cpu_percent(percpu=True)

        self.mem = psutil.virtual_memory()
        self.disk = psutil.disk_usage('/')

        self.netio = psutil.net_io_counters()

        self.netup = self.netio.bytes_sent - self.lastnetup
        self.netdw = self.netio.bytes_recv - self.lastnetdw

        self.lastnetup = self.netio.bytes_sent
        self.lastnetdw = self.netio.bytes_recv

        self.netup = self.netup / 1000
        self.netdw = self.netdw / 1000

    def send(self):
        """Send Data"""

        self.metrics.cores = int(self.cores)

        self.metrics.cpuTotal = float(self.cpuTotal)

        self.metrics.core0 = float(self.cpu[0])
        self.metrics.core1 = float(self.cpu[1])
        self.metrics.core2 = float(self.cpu[2])
        self.metrics.core3 = float(self.cpu[3])
        self.metrics.core4 = float(self.cpu[4])
        self.metrics.core5 = float(self.cpu[5])
        self.metrics.core6 = float(self.cpu[6])
        self.metrics.core7 = float(self.cpu[7])
        self.metrics.core8 = float(self.cpu[8])
        self.metrics.core9 = float(self.cpu[9])
        self.metrics.core10 = float(self.cpu[10])
        self.metrics.core11 = float(self.cpu[11])

        self.metrics.mem_percent = self.mem.percent
        self.metrics.mem_total = self.mem.total/1000000
        self.metrics.mem_available = self.mem.available/1000000
        self.metrics.disk_percent = self.disk.percent
        self.metrics.net_up = self.netup/1000
        self.metrics.net_dw = self.netdw/1000

        # print(self.metrics.SerializeToString())
        self.ser.write(self.metrics.SerializeToString())


def main():
    monitor = Monitor()

    while True:
        monitor.update()
        monitor.send()
        sleep(0.1)


if __name__ == '__main__':
    main()
