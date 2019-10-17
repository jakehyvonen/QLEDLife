import csv
import numpy as np
import serial
import sys
import time
from threading import *

# TODO:
#   exit if serial breaks down?
#   less latency in thread killing

class Pixel:
    def __init__(self,ch,ndv):
        self.ser = None
        self.running = False
        self.id = 100
        self.ch = ch
        self.dv = str(chr(ndv+65))
        self.current = 0
        self.start_time = None
        self.period = 10 #seconds
        self.name = 'px%02d%s' % (ch,self.dv)
        self.filename = None
        self.thread = None
        
    def set_current(self,current):
        self.current = current
        self.ser.write(b'setDac%02d%s%.02f\n' % (self.ch, self.dv, self.current))
        
    
    def start(self):
        if (self.running):
            print('Pixel Error: Already running')
        elif (self.id == -1):
            print('Pixel Error: Cannot begin test. Bad ID')
        # elif (self.ch < 0 or self.ch > 15):
            # print('Pixel Error: Cannot begin test. Bad channel')
        # elif (self.dv == -1):
            # print('PixelTest Error: Cannot begin test. Bad device')
        else:
            self.running = True
            self.start_time = time.time()
            self.filename = '%s-%s-%04d.csv' % (time.strftime('%Y-%m-%d-%H-%M-%S',time.localtime()),self.name,self.id)
            self.thread = Thread(target=self.test_loop)
            self.thread.name = self.name
            self.thread.start()
            
    def stop(self):
        if self.running:
            self.running = False
            
    def test_loop(self):
        print('%s Starting' % self.name)
        while(self.running):
            time.sleep(self.period)
            self.ser.write(b'getLed%02d%s\n' % (self.ch, self.dv))
            print('%s SEND: getLed%02d%s' % (self.name, self.ch, self.dv))
            while(self.ser.in_waiting):
                pass
            vled = float(self.ser.readline())
            print('%s RECV: %s' % (self.name,vled))
            # self.ser.write(b'getPd%02d%s\n' % (self.ch, self.dv))
            # while(self.ser.in_waiting):
                # pass
            # vpd = self.ser.readline()
            vpd = 0
            self.store_measurement(vled,vpd)
        print('%s Exiting' % self.name)
        
    def store_measurement(self,vled,vpd):
        elapsed_time = time.time() - self.start_time
        row = [elapsed_time,str(vled),str(vpd)]
        with open(self.filename,'a') as fd:
            writer = csv.writer(fd)
            writer.writerow(row)