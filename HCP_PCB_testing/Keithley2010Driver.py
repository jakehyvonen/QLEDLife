import PrologixDriver as comm
from decimal import Decimal as dec

def SendMsg(message):
    return comm.SendSerialCommandAndGetResponse(message)

def SendMsgGetResponse():
    var = input('enter command:')
    serialResponse = SendMsg(var)
    print('serialResponse:'+serialResponse)

def GPIBActivate():
    SendMsg('++addr 16')#set GPIB address to 15     

def Initialize():
    GPIBActivate()
    SendMsg('*RST')#reset instrument
    SendMsg(':CONF:CURR:DC')#set to measure DC current

def MeasureCurrent():
    GPIBActivate()
    SendMsg(':READ?')
    rawReading = comm.ReadGPIBMessage()
    print('rawReading: ' + rawReading)
    return dec(rawReading)
    
def ParseCurrentMeasurementString(s):
    print('hiya')

print('initializing KE2010Driver')
Initialize()
""" while(True):
    #SendMsgGetResponse()
    MeasureCurrent() """