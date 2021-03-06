import csv
import numpy as np
import serial
import sys
import time
from threading import *

ser_lock = Lock()

# TODO:
#   exit if serial breaks down?
#   less latency in thread killing

class Pixel:
    def __init__(self,dv,nch):
        self.ser = None
        self.running = False
        self.id = -1
        self.dv = dv
        self.ch = str(chr(nch+65))
        self.current = 0
        self.start_time = None
        self.period = 10 #seconds
        self.name = 'px%02d%s' % (dv,self.ch)
        self.filename = None
        self.thread = None
    
    def start(self,id,current):
        if (self.running):
            print('Pixel Error: Already running')
            return -1
        #elif (self.id == -1):
            # print('Pixel Error: Cannot begin test. Bad ID')
            # return -1
        # elif (self.ch < 0 or self.ch > 15):
            # print('Pixel Error: Cannot begin test. Bad channel')
        # elif (self.dv == -1):
            # print('PixelTest Error: Cannot begin test. Bad device')
        else:
            self.running = True
            self.set_id(id)
            self.set_current(current)
            self.start_time = time.time()
            self.filename = '%s-%s-%05d.csv' % (time.strftime('%Y-%m-%d-%H-%M-%S',time.localtime()),self.name,self.id)
            self.thread = Thread(target=self.test_loop)
            self.thread.name = self.name
            self.thread.start()
            
    def stop(self):
        if self.running:
            self.running = False
            
    def test_loop(self):
        global ser_lock
        print('%s Starting' % self.name)
        while(self.running):
            time.sleep(self.period)
            vled, vpd = self.get_measurements
            self.store_measurement(vled,vpd)
        print('%s Exiting' % self.name)
        self.exit(self);
        
    def set_current(self,current):
        self.current = current
        ser_lock.acquire()
        print('%s aquired serial lock' % self.name)
        self.ser.write(b'setDev%02d%s%.02f\n' % (self.dv, self.ch, self.current))
        print('%s SEND: setDev%02d%s%.02f' % (self.name, self.dv, self.ch, self.current))
        while(self.ser.in_waiting):
            pass
        err = self.ser.readline().strip()
        print('%s RECV: %s' % (self.name,err))
        ser_lock.release()
        print('%s has released lock' % self.name)
        
    def set_id(self,id):
        self.id = id
        ser_lock.acquire()
        print('%s aquired serial lock' % self.name)
        self.ser.write(b'setId%02d%s%05d\n' % (self.dv, self.ch, self.id))
        print('%s SEND: setId%02d%s%05d' % (self.name, self.dv, self.ch, self.id))
        while(self.ser.in_waiting):
            pass
        err = self.ser.readline().strip()
        print('%s RECV: %s' % (self.name,err))
        ser_lock.release()
        print('%s has released lock' % self.name)
        
    def get_measurements(self):
        ser_lock.acquire()
        print('%s aquired serial lock' % self.name)
        self.ser.write(b'getLed%02d%s\n' % (self.dv, self.ch))
        print('%s SEND: getLed%02d%s' % (self.name, self.dv, self.ch))
        while(self.ser.in_waiting):
            pass
        vled = self.ser.readline().strip()
        print('%s RECV: %s' % (self.name,vled))
        self.ser.write(b'getPd%02d%s\n' % (self.dv, self.ch))
        print('%s SEND: getPd%02d%s' % (self.name, self.dv, self.ch))
        while(self.ser.in_waiting):
            pass
        vpd = self.ser.readline().strip()
        print('%s RECV: %s' % (self.name,vpd))
        ser_lock.release()
        print('%s has released lock' % self.name)
        return vled, vpd
        
    def store_measurement(self,vled,vpd):
        elapsed_time = time.time() - self.start_time
        row = [elapsed_time,str(vled),str(vpd)]
        with open(self.filename,'a') as fd:
            writer = csv.writer(fd)
            writer.writerow(row)