import Tkinter as tk
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
from threading import *

EMULATOR_MODE = False

is_running = False

ser = serial.Serial()
ser.baudrate = 9600
ser.port = None
ser.timeout = 1

f = Figure(figsize=(5,5),dpi=100)
a = f.add_subplot(111)

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
        control_panel = ControlPanel(container, self)
        self.frames[ControlPanel] = control_panel
        control_panel.grid(row=0, column=0, sticky="nsew")
        
        plot_panel = PlotPanel(container, self)
        self.frames[PlotPanel] = plot_panel
        plot_panel.grid(row=0, column=1, sticky="nsew")
        
class ControlPanel(tk.Frame):
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
        
        self.current_entry = tk.Entry(
            self, width=6)
        self.current_entry.grid(row=1,column=2)
        
        self.connect_button = tk.Button(
            self, text="Connect", fg="blue", command=self.connect_to_controller)
        self.connect_button.grid(row=1,column=3)
        
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
        global is_running
        if ser.is_open:
            self.set_all()
            time.sleep(1)
            ser.write(b'start\n')
            if not is_running:
                is_running = True
                timer = Timer(5.0,get_measurement,[0])
                timer.start()
        else:
            print("Not connected")
            
    def stop(self):
        if ser.is_open:
            ser.write(b'quit\n')
            is_running = False
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
                
class PlotPanel(tk.Frame):
    def __init__(self,parent,controller):
        tk.Frame.__init__(self,parent)
        
        canvas = FigureCanvasTkAgg(f,self)
        canvas.show()
        canvas.get_tk_widget().pack(side=tk.BOTTOM,fill=tk.BOTH,expand=True)
        
        toolbar = NavigationToolbar2TkAgg(canvas, self)
        toolbar.update()
        canvas._tkcanvas.pack(side=tk.TOP, fill=tk.BOTH, expand=True)
        
def get_measurement(ch):
    global ser
    global is_running
    if is_running:
        dv = 0
        ser.write(b'getLed%02dA\n' % ch)
        print('GUI SEND: getLed%02dA' % ch)
        while(ser.in_waiting):
            pass
        vled = ser.readline()
        store_measurement('test.csv',ch,dv,vled,0)
        print('GUI RECV: ' + vled)
        timer = Timer(5.0,get_measurement,[(ch+1)%16])
        timer.start()
    else:
        print('not running')
        
def store_measurement(file,ch,dv,vled,vpd):
    #TODO - long or wide format?
    with open(file,'w') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow([vled])

    
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
        