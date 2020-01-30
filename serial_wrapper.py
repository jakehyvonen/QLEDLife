import sys
import glob
import serial
import time
from threading import *

class SerialWrapper:
    def __init__(self):
        self.ser = serial.Serial()
        self.ser.baudrate = 9600
        self.ser.port = None
        self.ser.timeout = 1
        self.ser_lock = Lock()
        self.is_open = False
        
    def transfer(self,buf):
        self.ser_lock.acquire()
        print('%s aquired serial lock' % current_thread())
        self.ser.write(buf)
        print('%s SEND: %s' % (current_thread(), buf))
        while(self.ser.in_waiting):
            pass
        ret = self.ser.readline().strip()
        print('%s RECV: %s' % (current_thread(),ret))
        self.ser_lock.release()
        print('%s has released lock' % current_thread())
        return ret
        
    def open(self):
        """ Lists serial port names

            :raises EnvironmentError:
                On unsupported or unknown platforms
            :returns:
                A list of the serial ports available on the system
        """
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')
        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        for port in result:
            try:
                print('Opening ' + port)
                self.ser.port = port
                self.ser.open()
                time.sleep(3)
                self.ser.flush()
                self.ser.write(b'ping')
                ret = self.ser.read(10)
                if (ret == 'pong'):
                    self.is_open = True
                    return 0                
            except (OSError, serial.SerialException):
                pass
        return -1

    def close(self):
        self.ser.close()
        self.ser.is_open = False

if __name__ == '__main__':
    ser = SerialWrapper()
    print('Success' if ser.open() == 0 else 'Failed')
    print(ser.transfer(b'setDev00A1.00\n'))