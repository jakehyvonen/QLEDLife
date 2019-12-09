import serial
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

# if(ser.isOpen):
#     ser.close()
#     ser.open()
    
def SendSerialCommand(command, eoiChar='\n'):
    ser.write((command+eoiChar).encode())
def SendSerialCommandAndGetResponse(command, eoiChar='\n'):
    #print('command: ' + command)
    #command = command.strip('\'')
    ser.write((command+eoiChar).encode())
    return ser.readline().decode()
def ReadGPIBMessage():
    return SendSerialCommandAndGetResponse('++read')
def GetRawInput():
    var = input('enter command:').strip(' ')
    print(SendSerialCommandAndGetResponse(var))
#SendSerialCommand('++auto 0')#ensure driver is in Controller mode
#while(True):
#    GetRawInput()