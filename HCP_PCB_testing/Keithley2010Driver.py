import PrologixDriver as comm

def SendMsg(message):
    return comm.SendSerialCommandAndGetResponse(message)

def SendMsgGetResponse():
    var = input('enter command:')
    serialResponse = SendMsg(var)
    print('serialResponse:'+serialResponse)

def Initialize():
    SendMsg('++addr 16')#set GPIB address to 16 (KE2010 default)
    #SendMsg('REN')#enable remote mode
    SendMsg('*RST')#reset instrument
    SendMsg(':CONF:CURR:DC')#set to measure DC current

def MeasureCurrent():
    SendMsg(':READ?')
    rawReading = comm.ReadGPIBString()
    print('rawReading: ' + rawReading)
    
def ParseCurrentMeasurementString(s):
    print('hiya')

print('initializing KE2010Driver')
Initialize()
while(True):
    #SendMsgGetResponse()
    MeasureCurrent()