import sys
import glob
import serial
import time

def serial_ports():
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
    return result
    
def ping_controller(ports):
    for port in ports:
        try:
            s = serial.Serial(port,9600,timeout=1)
            time.sleep(3)
            s.flush()
            s.write(b'ping')
            ret = s.read(10)
            print(ret)
            if ret == 'pong':
                print(s.name)
                s.close()
                return port
            else:
                s.close()
        except (OSError, serial.SerialException):
            pass
    return -1
    
if __name__ == '__main__':
    print(serial_ports())