#include <Arduino.h>
#include <SPI.h>
#include "DAC.h"
#include "defines.h"
#include "dac70004.h"
#include "MemoryOperations.h"

Dac::Dac(int maxCode, float maxCurrent, int nDevice, int syncPin) : MAX_DAC_CODE(maxCode), MAX_CURRENT(maxCurrent), NUM_DEVICES(nDevice), SYNC_PIN(syncPin){};

/* NEEDED FOR digitalDACWrite() */
const int DACSelectPin = 8;
SPISettings settingsB(9600, MSBFIRST, SPI_MODE2);

void Dac::begin()
{
  this->settingsA = SPISettings(9600, MSBFIRST, SPI_MODE2);
  SPI.begin();
  pinMode(SYNC_PIN, OUTPUT);
  delay(10);
  digitalWrite(SYNC_PIN, HIGH);
  this->enable_SDO();
};

void Dac::transmit(byte *tx_buf, byte *rx_buf, int len)
{
  digitalWrite(SYNC_PIN,LOW);
  SPI.beginTransaction(this->settingsA);

  for (int i = 0; i < len; ++i)
  {
    rx_buf[i] = SPI.transfer(tx_buf[i]);
  }

  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();
}

int Dac::current_float_to_int(float f)
{
  float currentVal = f * MAX_DAC_CODE / MAX_CURRENT;
  int DACint = (int)round(currentVal);
  return DACint;
}

float Dac::current_int_to_float(int i)
{
  float currentFloat = ((float)i / (float)MAX_DAC_CODE) * MAX_CURRENT;
  return currentFloat;
}

void Dac::send_nop(byte* buf)
{
  byte cmdBytes[4];
  cmdBytes[0] = DAC_NOP_CMD;
  cmdBytes[1] = 0;
  cmdBytes[2] = 0;
  cmdBytes[3] = 0;
  
  buf[0] = SPI.transfer(cmdBytes[0]);
  buf[1] = SPI.transfer(cmdBytes[1]);
  buf[2] = SPI.transfer(cmdBytes[2]);
  buf[3] = SPI.transfer(cmdBytes[3]);
}

void Dac::enable_SDO()
{
  int len = 4;
  byte tx_buf[len];
  byte rx_buf[len];

  tx_buf[0] = DAC_SDO_CMD;
  tx_buf[1] = 0;
  tx_buf[2] = 0;
  tx_buf[3] = DAC_SDO_EN;
  
  for (int i = 0; i < NUM_DEVICES; i++)
  {
    transmit(tx_buf,rx_buf,len);    
    delay(1);
  }
}

void Dac::set_all_current(float f)
{
  int DACSetInt = current_float_to_int(f);
  
  for (int dv = 0; dv < NUM_DEVICES; dv++)
  {
    store_dac_int(dv,CHANNEL_A,DACSetInt);
    store_dac_int(dv,CHANNEL_B,DACSetInt);
    store_dac_int(dv,CHANNEL_C,DACSetInt);
    store_dac_int(dv,CHANNEL_D,DACSetInt);
  };

  int len = NUM_DEVICES*4;
  byte tx_buf[len];
  byte rx_buf[len];

  for (int i = 0; i < len; i+=4)
  {
    tx_buf[i] = DAC_WRITE_BUF_UPDATE_ALL_CMD;
    tx_buf[i+1] = DAC_CHANNEL_ALL | (DACSetInt >> 10);
    tx_buf[i+2] = (DACSetInt >> 2) & 0xFF;
    tx_buf[i+3] = (DACSetInt & 0xFF) << 6;
  }
  
  transmit(tx_buf,rx_buf,len);
}

void Dac::set_current(int device, int channel, float f)
{
  if (channel < 0 || channel > 3)
  {
    return;
  }
  
  int DACSetInt = current_float_to_int(f);

  store_dac_int(device,channel,DACSetInt);

  int len = NUM_DEVICES*4;
  byte tx_buf[len];
  byte rx_buf[len];

  device = NUM_DEVICES - 1;
  for (int i = 0; i < len; i+=4)
  {
    DACSetInt = get_dac_int(device--,channel);
    
    tx_buf[i] = DAC_WRITE_BUF_UPDATE_ALL_CMD;
    tx_buf[i+1] = (channel << 4) | (DACSetInt >> 10);
    tx_buf[i+2] = (DACSetInt >> 2) & 0xFF;
    tx_buf[i+3] = (DACSetInt & 0xFF) << 6;
  }

  transmit(tx_buf,rx_buf,len);
}

float Dac::read_device_current(int device, int channel)
{
  byte buf[4];
  
  byte cmdBytes[4];
  cmdBytes[0] = DAC_READ + DAC_WRITE_BUF_CMD;
  cmdBytes[1] = (channel << 4) & 0xFF;
  cmdBytes[2] = B00000000;
  cmdBytes[3] = B00000000;

  /*
  Serial.println(cmdBytes[0],BIN);
  Serial.println(cmdBytes[1],BIN);
  Serial.println(cmdBytes[2],BIN);
  Serial.println(cmdBytes[3],BIN);
  */

  digitalWrite(SYNC_PIN,LOW);
  delay(10);
  SPI.beginTransaction(this->settingsA);

  for (int i = device; i < NUM_DEVICES-1; i++)
  {
    send_nop(&buf[0]);
  }

  SPI.transfer(cmdBytes[0]);
  SPI.transfer(cmdBytes[1]);
  SPI.transfer(cmdBytes[2]);
  SPI.transfer(cmdBytes[3]);
  
  for (int i = 0; i < device; i++)
  {
    send_nop(&buf[0]);
  }
  
  delay(10); //arbitrary delay to ensure data transfer is not interrupted
  digitalWrite(SYNC_PIN,HIGH);
  SPI.endTransaction();

  delay(20);

  digitalWrite(SYNC_PIN,LOW);
  delay(10);
  SPI.beginTransaction(this->settingsA);
  

  for (int i = device; i < NUM_DEVICES; i++)
  {
    send_nop(&buf[0]);
  }

  unsigned int currentInt = buf[1] & 0x0F;
  currentInt = (currentInt << 8) + buf[2];
  currentInt = (currentInt << 2) + (buf[3] >> 6);

  for (int i = 0; i < device; i++)
  {
    send_nop(&buf[0]);
  }
  
  delay(10); //arbitrary delay to ensure data transfer is not interrupted
  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();

  float currentFloat = ((float)currentInt / (float)MAX_DAC_CODE) * MAX_CURRENT;

  return currentFloat;
}

/* ######################################################
*                         TEST fn
*         These functions work. Use to debug.
*  ######################################################
*/

void Dac::test_write() {
  int address = 0;
  int value = 0x2000;
  
  byte commandByte3 = B00000011; //datasheet: command for 'write to buffer and update DAC'
  //these addresses include 0000 to prepare for | operation with int16 value
  byte addressByte[4] =
  {
    B00000000, //DAC channel A (NPI Site A)
    B00010000, //DAC channel B (NPI Site B)
    B00100000, //DAC channel C (NPI SiteC)
    B00110000 //DAC channel D (NPI SiteD)
  };
  byte commandByte2 = addressByte[address] | (value >> 10); //need first 4 bits from int16 value
  byte commandByte1 = (value >> 2) & 0xFF; //0xFF == B11111111, get next 8 bits by shifting right 2 places
  byte commandByte0 = (value & 0xFF)<<6;//then cut off left 6 to conform to register format to get final 2 bits
  Serial.print("addressByte = ");
  Serial.println(addressByte[address], BIN);
  Serial.print("commandByte3 = ");
  Serial.println(commandByte3, BIN);
  Serial.print("commandByte2 = ");
  Serial.println(commandByte2, BIN);
  Serial.print("commandByte1 = ");
  Serial.println(commandByte1, BIN);
  Serial.print("commandByte0 = ");
  Serial.println(commandByte0, BIN);
  //initialize the SPI transaction
  SPI.beginTransaction(settingsB);
  // take the SS pin low to select the chip:
  digitalWrite(DACSelectPin,LOW);
  delay(10);
  for (int i = 0; i < NUM_DEVICES; i++)
  {
    Serial.print("tx ");Serial.println(i);
    SPI.transfer(commandByte3);
    SPI.transfer(commandByte2);
    SPI.transfer(commandByte1);
    SPI.transfer(commandByte0);
  }
  // take the SS pin high to de-select the chip:
  delay(10);
  digitalWrite(DACSelectPin,HIGH); 
  SPI.endTransaction();
}

void Dac::test_enable_SDO()
{
  byte commandByte3 = B00001000;
  byte commandByte2 = B00000000;
  byte commandByte1 = B00000000;
  byte commandByte0 = B00000010;
  digitalWrite(SYNC_PIN,LOW); 
  delay(10);
  SPI.beginTransaction(this->settingsA);
  for (int i = 0; i < NUM_DEVICES; i++)
  {
    Serial.print("SDO en ");Serial.println(i);
    SPI.transfer(commandByte3);
    SPI.transfer(commandByte2);
    SPI.transfer(commandByte1);
    SPI.transfer(commandByte0);
  }
  delay(10); //arbitrary delay to ensure data transfer is not interrupted
  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();
}

void Dac::test_clr()
{
  byte commandByte3 = B00001011;
  byte commandByte2 = B00000000;
  byte commandByte1 = B00000000;
  byte commandByte0 = B00000010;
  digitalWrite(SYNC_PIN,LOW); 
  delay(10);
  SPI.beginTransaction(this->settingsA);
  SPI.transfer(commandByte3);
  SPI.transfer(commandByte2);
  SPI.transfer(commandByte1);
  SPI.transfer(commandByte0);
  delay(10); //arbitrary delay to ensure data transfer is not interrupted
  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();
}

void Dac::test_read_register(byte reg, int ch, byte* buf)
{
  byte cmdBytes[4];
  cmdBytes[0] = DAC_READ + reg;
  cmdBytes[1] = (ch << 4) & 0xFF;
  cmdBytes[2] = B00000000;
  cmdBytes[3] = B00000000;

  digitalWrite(SYNC_PIN,LOW);
  delay(10);
  SPI.beginTransaction(this->settingsA);

  for (int i = 0; i < NUM_DEVICES; i++)
  {
    SPI.transfer(cmdBytes[0]);
    SPI.transfer(cmdBytes[1]);
    SPI.transfer(cmdBytes[2]);
    SPI.transfer(cmdBytes[3]);
  }
  
  delay(10); //arbitrary delay to ensure data transfer is not interrupted
  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();

  delay(20);

  digitalWrite(SYNC_PIN,LOW);
  delay(10);
  SPI.beginTransaction(this->settingsA);

  for (int i = 0; i < NUM_DEVICES; i++)
  {
    Serial.print(i);Serial.println("th NOP");
    send_nop(&buf[i*4]);
    Serial.println(buf[(i<<2)],BIN);
    Serial.println(buf[(i<<2)+1],BIN);
    Serial.println(buf[(i<<2)+2],BIN);
    Serial.println(buf[(i<<2)+3],BIN);
  }
  
  delay(10); //arbitrary delay to ensure data transfer is not interrupted
  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();
}
