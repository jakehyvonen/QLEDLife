import PrologixDriver as comm
from time import sleep

#ToDo: store prologix params for each instrument in prologixDriver
#eoi = 1 (assert EOI signal = true)
#eos = 1 (CR)

def SendMsg(message):
    return comm.SendSerialCommandAndGetResponse(message)

def SendMsgGetResponse():
    var = input('enter command:')
    serialResponse = SendMsg(var)
    print('serialResponse:'+serialResponse)
    #readResponse = comm.ReadGPIBMessage()
    #print('readResponse:'+readResponse)

def Initialize():
    SendMsg('++addr 15')#set GPIB address to 15     
    #SendMsg('F0,0X')#(THIS CAUSES HANGUP)set source: voltage, measure: current
    SendMsg('B0.0,0,0X')#set bias 0V, autorange, no delay
    SendMsg('G4,0,0X')#output data: measure value, ASCII, one line of data
    SendMsg('P4X')#average 16 readings

def TurnOutputOn():
    SendMsg('N1X')

def TurnOutputOff():
    SendMsg('N0X')
    SetVoltage(0.0)

def SetVoltage(voltage):
    val = str(voltage)
    SendMsg('B'+val+',0,0X')

def MeasureCurrent():
    response = comm.ReadGPIBMessage()
    return ParseCurrentString(response)

def ParseCurrentString(s):
    if('+'in s):
        s=s.split('+',1)[-1]
    if('I'in s):
        s=s.split('I',1)[-1]
    if(','in s):
        s=s.split(',')[0]
    return s
    
Initialize()
while(True):
    SendMsgGetResponse()