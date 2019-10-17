import Tkinter as tk
import ttk
import tkMessageBox

import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation

import csv
import serial_connection as sc
import serial
import sys
import time
import numpy as np

from emulator import *
from pixel import *
from threading import *

EMULATOR_MODE = False

ser = serial.Serial()
ser.baudrate = 9600
ser.port = None
ser.timeout = 1

#f = Figure(figsize=(5,5),dpi=100)
#a = f.add_subplot(111)

# def animate(i):
    # xList = []
    # yList = []
    # i = 0
    # with open('test.csv') as csvfile:
            # csvreader = csv.reader(csvfile)
            # for row in csvreader:
                # yList.append(float(''.join(row)))
                # xList.append(i)
                # i += 1
    # a.clear()
    # a.plot(xList, yList)


class App(tk.Tk):            
    def __init__(self,*args,**kwargs):
        tk.Tk.__init__(self,*args,**kwargs)
        
        container = tk.Frame(self)
        container.pack(side="top",fill="both",expand=True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)
        
        self.frames = {}
        status_panel = StatusPanel(container, self)
        self.frames[StatusPanel] = status_panel
        status_panel.grid(row=0, column=0, sticky="nsew")
        
        control_panel = ControlPanel(container, self)
        self.frames[ControlPanel] = control_panel
        control_panel.grid(row=0, column=1, sticky="nsew")
        
class StatusPanel(tk.Frame):
    def __init__(self,parent,controller):
        tk.Frame.__init__(self,parent)
        
        self.connection_status_label = tk.Label(
            self,fg="red",text="Not connected")
        self.connection_status_label.grid(columnspan=4,sticky=tk.W)
        
        self.off_button = tk.Button(
            self, text="OFF", command=self.set_all_off)
        self.off_button.grid(row=1)
        
        self.set_button = tk.Button(
            self, text="SET", fg="red", command=self.set_all)
        self.set_button.grid(row=1,column=1)
        
        self.connect_button = tk.Button(
            self, text="Connect", fg="blue", command=self.connect_to_controller)
        self.connect_button.grid(row=1,column=3)
        
        self.channel_entry = tk.Entry(
            self, width=6)
        self.channel_entry.grid(row=2,column=1)
        
        self.device_entry = tk.Entry(
            self, width=6)
        self.device_entry.grid(row=2,column=2)
        
        self.start_button = tk.Button(
            self, text="Start", fg="green", command=self.start)
        self.start_button.grid(row=10,rowspan=2,columnspan=2,sticky=tk.W+tk.E)
        
        self.stop_button = tk.Button(
            self, text="Stop", fg="red", command=self.stop)
        self.stop_button.grid(row=10,column=2,columnspan=2,sticky=tk.W+tk.E)
        
        
    def set_all_off(self):
        if ser.is_open:
            ser.write(b'setAll0.00\n')
        else:
            print("Not connected")

    def set_all(self):
        if ser.is_open:
            entry = self.current_entry.get()
            if (entry == ''):
                entry = '0.00'
            ser.write(('setAll' + entry + '\n').encode())
        else:
            print("Not connected")
            
    def start(self):
        global ser
        if ser.is_open:
            ch = int(self.channel_entry.get())
            dv = ord(self.device_entry.get())-65
        else:
            print("Not connected")
            
    def stop(self):
        if ser.is_open:
            ch = int(self.channel_entry.get())
            dv = ord(self.device_entry.get())-65
        else:
            print("Not connected")
        

    def connect_to_controller(self):
        global ser
        if ser.is_open:
            print("Already connected.")
            return
        elif EMULATOR_MODE:
            ser = serial.serial_for_url('loop://')
            em_thread = Thread(target = emulator_init)
            em_thread.start()
            port = 'EMULATOR'
        else:
            print('Attempting to connect...')
            result = sc.serial_ports()
            print(result)
            port = sc.ping_controller(result)
            if port == -1:
                print('Connection failed')
                self.connection_status_label.config(fg="red",text="Not connected. (attempt failed)")
                return -1    
            else:
                ser.port = port
                ser.open()
        if ser.is_open:
            print('Connected at ' + port)
            self.connection_status_label.config(fg="green",text="Connected")
            return 0
        else:
            print('Connection failed')
            self.connection_status_label.config(fg="red",text="Not connected. (attempt failed)")
            return -1
                
class ControlPanel(ttk.Notebook):
    def __init__(self,parent,controller):
        ttk.Notebook.__init__(self,parent)
        tab0 = DeviceFrame(self,0)
        tab1 = DeviceFrame(self,1)
        tab2 = DeviceFrame(self,2)
        tab3 = DeviceFrame(self,3)
        tab4 = DeviceFrame(self,4)
        tab5 = DeviceFrame(self,5)
        tab6 = DeviceFrame(self,6)
        tab7 = DeviceFrame(self,7)
        tab8 = DeviceFrame(self,8)
        tab9 = DeviceFrame(self,9)
        tab10 = DeviceFrame(self,10)
        tab11 = DeviceFrame(self,11)
        tab12 = DeviceFrame(self,12)
        tab13 = DeviceFrame(self,13)
        tab14 = DeviceFrame(self,14)
        tab15 = DeviceFrame(self,15)
        self.add(tab0, text = "Dev 0")
        self.add(tab1, text = "Dev 1")
        self.add(tab2, text = "Dev 2")
        self.add(tab3, text = "Dev 3")
        self.add(tab4, text = "Dev 4")
        self.add(tab5, text = "Dev 5") 
        self.add(tab6, text = "Dev 6")
        self.add(tab7, text = "Dev 7")
        self.add(tab8, text = "Dev 8")  
        self.add(tab9, text = "Dev 9")
        self.add(tab10, text = "Dev 10")
        self.add(tab11, text = "Dev 11")
        self.add(tab12, text = "Dev 12")
        self.add(tab13, text = "Dev 13") 
        self.add(tab14, text = "Dev 14")
        self.add(tab15, text = "Dev 15")

class DeviceFrame(tk.Frame):
    def __init__(self,parent,device):
        tk.Frame.__init__(self,parent)
        
        self.pixelA = PixelFrame(self,device,0)
        self.pixelA.grid(row=0,sticky=tk.W+tk.E)
        self.pixelB = PixelFrame(self,device,1)
        self.pixelB.grid(row=1,sticky=tk.W+tk.E)
        self.pixelC = PixelFrame(self,device,2)
        self.pixelC.grid(row=2,sticky=tk.W+tk.E)
        self.pixelD = PixelFrame(self,device,3)
        self.pixelD.grid(row=3,sticky=tk.W+tk.E)
        
        self.start_button = tk.Button(
            self, text="START ALL", fg="green", command=self.start_all)
        self.start_button.grid(row=4,column=1)
        
        self.stop_button = tk.Button(
            self, text="STOP ALL", fg="red", command=self.stop_all)
        self.stop_button.grid(row=4,column=2)

    def start_all(self):
        self.pixelA.start()
        time.sleep(1)
        self.pixelB.start()
        time.sleep(1)
        self.pixelC.start()
        time.sleep(1)
        self.pixelD.start()
        
    def stop_all(self):
        self.pixelA.stop()
        self.pixelB.stop()
        self.pixelC.stop()
        self.pixelD.stop()


class PixelFrame(tk.Frame):
    def __init__(self,parent,device,px):
        tk.Frame.__init__(self,parent)
        
        self.pixel = Pixel(device,px)
        
        self.pixel_label = tk.Label(
            self,fg="red",text=(('Pixel %02d' % device) + str(chr(px+65))))
        self.pixel_label.grid(sticky=tk.W)
        
        self.current_entry_label = tk.Label(
            self,text='Set current:')
        self.current_entry_label.grid(row=1)
        
        self.current_entry = tk.Entry(
            self, width=6)
        self.current_entry.grid(row=1,column=1)
        
        # self.time_running_label = tk.Label(
            # self,text='Time: ')
        # self.time_running_label.grid(row=1,column=2)
        
    def start(self):
        global ser
        self.pixel_label.configure(fg='green')
        self.pixel.ser = ser
        self.pixel.set_current(float(self.current_entry.get()))
        self.pixel.start()
        
    def stop(self):
        self.pixel_label.configure(fg='red')
        self.pixel.stop()
        

    
def emulator_init():
    global ser
    em = Emulator(ser)
    print('GUI: Emulator exited')
    ser = serial.Serial()

if __name__ == '__main__':
    if sys.argv[1] == '-t' or sys.argv[1] == '-e':
        EMULATOR_MODE = True
    app = App()
    # ani = animation.FuncAnimation(f, animate, interval=1000)
    app.mainloop()
    app.destroy()
        