#include <Wire.h>
#include <Adafruit_ADS1015.h> //ADS1115 library

#define AD5645R_BROADCAST_ADDR   (0x8)
#define AD5645R_DAC_CHANNEL_A    (0x0)
#define AD5645R_DAC_CHANNEL_B    (0x1)
#define AD5645R_DAC_CHANNEL_C    (0x2)
#define AD5645R_DAC_CHANNEL_D    (0x3)
#define AD5645R_DAC_CHANNEL_ALL  (0x7)
#define AD5645R_CMD_WRITE_REG_N         (0x0 << 3)
#define AD5645R_CMD_UPDATE_REG_N        (0x1 << 3)
#define AD5645R_CMD_WRITE_UPDATE_ALL    (0x2 << 3)
#define AD5645R_CMD_WRITE_UPDATE_N      (0x3 << 3)
#define AD5645R_CMD_RESET               (0x5 << 3) 
#define m_maxVoltage 5.0
#define m_maxCode 16383

Adafruit_ADS1115 ADS1115(0x48);
float startVolt = 0.011;
float stepSize = 0.33; //volts
float stopVolt = 4.94;
float presentV;

void setup(){
  noInterrupts();
  Serial.begin(9600);
  ADS1115.setGain(GAIN_TWOTHIRDS);
  ADS1115.begin();
  presentV = startVolt;
  interrupts();
}
void loop(){  
  Serial.print("presentV: ");
  Serial.println(presentV);
  set_all_voltage(presentV);
  int result;
  result = ADS1115.readADC_SingleEnded(0);    // read LED voltage
  String measString = "meas0: ";
  measString.concat(String(result));
  measString.concat(" meas1: ");
  measString.concat(String(ADS1115.readADC_SingleEnded(1)));
  measString.concat(" meas2: ");
  measString.concat(String(ADS1115.readADC_SingleEnded(2)));
  measString.concat(" meas3: ");
  measString.concat(String(ADS1115.readADC_SingleEnded(3)));
  Serial.println(measString);
  delay(111);
  if(presentV < stopVolt)
    presentV += stepSize;
  else
    presentV = startVolt;
}

static void i2cwrite(uint8_t i2cAddress, uint8_t command, uint16_t data)
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(command);
  Wire.write((uint8_t)(data >> 6));
  Wire.write((uint8_t)((data << 2) & 0xFF));
  Wire.endTransmission();
}
int voltage_float_to_int(float f)
{
  float voltageVal = f * m_maxCode / m_maxVoltage;
  int DACint = (int)round(voltageVal);
  return DACint;
}
int set_all_voltage(float f)
{
  if (f > m_maxVoltage)
    return -1;
    
  int DacSetInt = voltage_float_to_int(f);

  if (DacSetInt > m_maxCode)
    return -1;

  //TODO - error check here
  uint8_t commandByte = AD5645R_CMD_WRITE_UPDATE_N | AD5645R_DAC_CHANNEL_ALL;
  i2cwrite(AD5645R_BROADCAST_ADDR,commandByte,DacSetInt);

  return 0;
}
