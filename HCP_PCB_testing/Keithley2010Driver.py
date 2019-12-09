import PrologixDriver as comm

def SendMsg(message):
    return comm.SendSerialCommandAndGetResponse(message)

def SendMsgGetResponse():
    var = input('enter command:')
    serialResponse = SendMsg(var)
    print('serialResponse:'+serialResponse)

def Initialize():
    SendMsg('++addr 16')#set GPIB address to 16 (KE2010 default)    
    SendMsg(':CONF:CURR:DC;')#set bias 0V, autorange, no delay
    SendMsg('G4,0,0X')#output data: measure value, ASCII, one line of data
    SendMsg('P4X')#average 16 readings