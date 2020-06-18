import os

import psutil
import socket
from time import sleep

import status_pb2

import sensors

PORT = "/dev/ttyACM0"
os.nice(0)


class Monitor:
    def __init__(self):
        """Initialize"""

        self.status = status_pb2.CpuStatus()
        sensors.init()

        self.host = '192.168.10.140'
        self.port = 1337

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(0.5)
        self.sock.connect((self.host, self.port))

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

        for chip in sensors.iter_detected_chips():
            print('%s at %s' % (chip, chip.adapter_name))
            for feature in chip:
                print('  %s: %.2f' % (feature.label, feature.get_value()))

        # for i, chip in enumerate(sensors.iter_detected_chips()):
        #     print('%s at %s' % (chip, chip.adapter_name))
        #     for j, feature in enumerate(chip):
        #         print('  %s: %.2f' % (feature.label, feature.get_value()))
        #         self.chips[i][j] = feature.get_value()

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

        self.status.cores = int(self.cores)

        self.status.cpuTotal = float(self.cpuTotal)

        self.status.core0 = float(self.cpu[0])
        self.status.core1 = float(self.cpu[1])
        self.status.core2 = float(self.cpu[2])
        self.status.core3 = float(self.cpu[3])
        self.status.core4 = float(self.cpu[4])
        self.status.core5 = float(self.cpu[5])
        self.status.core6 = float(self.cpu[6])
        self.status.core7 = float(self.cpu[7])
        self.status.core8 = float(self.cpu[8])
        self.status.core9 = float(self.cpu[9])
        self.status.core10 = float(self.cpu[10])
        self.status.core11 = float(self.cpu[11])

        self.status.mem_percent = self.mem.percent
        self.status.mem_total = self.mem.total / 1000000
        self.status.mem_available = self.mem.available / 1000000
        self.status.disk_percent = self.disk.percent
        self.status.net_up = self.netup / 1000
        self.status.net_dw = self.netdw / 1000

        msg = self.status.SerializeToString()
        self.sock.sendall(msg)

        try:
            data = self.sock.recv(1024)
            print(data)
        except socket.timeout as e:
            print(e)


def main():
    monitor = Monitor()

    while True:
        monitor.update()
        monitor.send()
        sleep(3)


if __name__ == '__main__':
    main()
