import numpy as np
import serial
import sys
import time
from threading import *

class Emulator:
    def __init__(self,ser):
        self.LedMeas = np.zeros((16,4))
        self.PdMeas = np.zeros((16,4))
        self.DataValid = np.zeros(16)
        self.DevId = np.zeros(16)
        self.DacSet = np.zeros((16,4))
        self.shutdown = False
        self.running = False
        self.ser = ser
        self.thread = None
            
    def start(self):
        if (self.running):
            print('EMU Error: Already running')
        # elif (self.ch < 0 or self.ch > 15):
            # print('Pixel Error: Cannot begin test. Bad channel')
        # elif (self.dv == -1):
            # print('PixelTest Error: Cannot begin test. Bad device')
        else:
            self.running = True
            timer = Timer(1.0,self.ADC_measure,[0])
            timer.start()
            self.thread = Thread(target=self.emulator_main)
            self.thread.start()
        
    def stop(self):
        if self.running:
            self.shutdown = True
        
    def ADC_measure(self,ch):
        if (self.running):
            # self.LedMeas[ch][0] = self.DacSet[ch][0] * 0.0003052
            # self.LedMeas[ch][1] = self.DacSet[ch][1] * 0.0003052
            # self.LedMeas[ch][2] = self.DacSet[ch][2] * 0.0003052
            # self.LedMeas[ch][3] = self.DacSet[ch][3] * 0.0003052
            self.LedMeas[ch][0] = self.LedMeas[ch][0]+(self.DacSet[ch][0] * 0.0003052)
            self.LedMeas[ch][1] = self.LedMeas[ch][1]+(self.DacSet[ch][1] * 0.0003052)
            self.LedMeas[ch][2] = self.LedMeas[ch][2]+(self.DacSet[ch][2] * 0.0003052)
            self.LedMeas[ch][3] = self.LedMeas[ch][3]+(self.DacSet[ch][3] * 0.0003052)
            self.DataValid[ch] = 1;
            #print(self.LedMeas[ch])
            timer = Timer(1.0,self.ADC_measure,[(ch+1)%16])
            timer.start()
    
    def parse_command(self,command):
        print('EMU RECV: ' + command)
        if (command[:6] == 'setAll'):
            dacSetting = int(16383 * float(command[6:10]) / 10.0)
            self.DacSet = np.full((16,4),dacSetting)
        elif (command[:6] == 'setDac'):
            dacSetting = int(16383 * float(command[9:13]) / 10.0)
            dv = int(command[6:8])
            px = ord(command[8:9]) - 65
            self.DacSet[dv][px] = dacSetting
        elif (command[:6] == 'setId'):
            id = int(command[9:14])
            dv = int(command[6:8])
            px = ord(command[8:9]) - 65
            self.DevId[dv][px] = id
        # elif (command[:5] == 'start'):
            # if (~self.running):
                # self.running = True
                # timer = Timer(1.0,self.ADC_measure,[0])
                # timer.start()
        # elif (command[:5] == 'stop'):
            # if (self.running):
                # self.running = False
        elif (command[:6] == 'getLed'):
            dv = int(command[6:8])
            px = ord(command[8:9]) - 65
            if self.DataValid[dv]:
                print('EMU SEND: %.6f' % self.LedMeas[dv][px])
                self.ser.write('%.6f\n' % self.LedMeas[dv][px])
            else:
                self.ser.write('Data not valid')
            while(self.ser.in_waiting):
                pass
        
    def emulator_main(self):
        while not self.shutdown:
            while (self.ser.in_waiting):
                command = self.ser.readline()
                self.parse_command(command)
        self.running = False
        print("EMU: Shutting down")