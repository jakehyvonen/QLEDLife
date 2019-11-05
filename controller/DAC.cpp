#include <Arduino.h>
#include <SPI.h>
#include "DAC.h"
#include "defines.h"

Dac::Dac(int maxCode, float maxCurrent, int nDevice, int syncPin) : MAX_DAC_CODE(maxCode), MAX_CURRENT(maxCurrent), NUM_DEVICES(nDevice), SYNC_PIN(syncPin){};

void Dac::begin()
{
  this->settingsA = SPISettings(9600, MSBFIRST, SPI_MODE2);
  SPI.begin();
  pinMode(SYNC_PIN, OUTPUT);
  delay(10);
  digitalWrite(SYNC_PIN, HIGH);
  this->enable_SDO();
  this->enable_SDO();
  this->enable_SDO();
  /*byte ret = this->SDO_status();
  while (ret != 2)
  {
    Serial.println("SDO enable failed!");
    ret = this->SDO_status();
  }
  */
  //this->set_all_current(CHANNEL_ALL,0.0);
};

void Dac::send_write_with_update(unsigned int address, unsigned int value)
{
  byte cmdBytes[4];
  cmdBytes[0] = B00000011;
  cmdBytes[1] = (address << 4) | (value >> 10);
  cmdBytes[2] = (value >> 2) & 0xFF;
  cmdBytes[3] = (value & 0xFF) << 6;

  byte rcvBytes[4] = {0,0,0,0};
  
  rcvBytes[0] = SPI.transfer(cmdBytes[0]);
  rcvBytes[1] = SPI.transfer(cmdBytes[1]);
  rcvBytes[2] = SPI.transfer(cmdBytes[2]);
  rcvBytes[3] = SPI.transfer(cmdBytes[3]);

  /*
  Serial.println("Sent:");
  Serial.println(cmdBytes[0],BIN);
  Serial.println(cmdBytes[1],BIN);
  Serial.println(cmdBytes[2],BIN);
  Serial.println(cmdBytes[3],BIN);
  
  Serial.println("Received:");
  Serial.println(rcvBytes[0],BIN);
  Serial.println(rcvBytes[1],BIN);
  Serial.println(rcvBytes[2],BIN);
  Serial.println(rcvBytes[3],BIN);
  */
}

void Dac::send_nop()
{
  byte cmdBytes[4];
  cmdBytes[0] = B00001110;
  cmdBytes[1] = B00000000;
  cmdBytes[2] = B00000000;
  cmdBytes[3] = B00000000;

  byte rcvBytes[4] = {0,0,0,0};
  
  rcvBytes[0] = SPI.transfer(cmdBytes[0]);
  rcvBytes[1] = SPI.transfer(cmdBytes[1]);
  rcvBytes[2] = SPI.transfer(cmdBytes[2]);
  rcvBytes[3] = SPI.transfer(cmdBytes[3]);

  /*
  Serial.println("Sent:");
  Serial.println(cmdBytes[0],BIN);
  Serial.println(cmdBytes[1],BIN);
  Serial.println(cmdBytes[2],BIN);
  Serial.println(cmdBytes[3],BIN);
  
  Serial.println("Received:");
  Serial.println(rcvBytes[0],BIN);
  Serial.println(rcvBytes[1],BIN);
  Serial.println(rcvBytes[2],BIN);
  Serial.println(rcvBytes[3],BIN);
  */
}

// HOPEFULLY will soon be deprecated
void Dac::digital_write(int address, int value)
{
  byte commandByte3 = B00000011;
  byte commandByte2 = (address << 4) | (value >> 10);
  byte commandByte1 = (value >> 2) & 0xFF;
  byte commandByte0 = (value & 0xFF)<<6;
  
  byte commandBytes[NUM_DEVICES*4];
  for (int i = 0; i < NUM_DEVICES; i++)
  {
    commandBytes[i*4+3] = commandByte3;
    commandBytes[i*4+2] = commandByte2;
    commandBytes[i*4+1] = commandByte1;
    commandBytes[i*4] = commandByte0;
  }
  digitalWrite(SYNC_PIN,LOW); 
  delay(10);
  SPI.beginTransaction(this->settingsA);
  for (int i = sizeof(commandBytes)-1; i>= 0; i--)
  {
    SPI.transfer(commandBytes[i]);
    //Serial.println(commandBytes[i],BIN);
  }
  /*
  SPI.transfer(commandByte3);
  SPI.transfer(commandByte2);
  SPI.transfer(commandByte1);
  SPI.transfer(commandByte0);
  */
  delay(10); //arbitrary delay to ensure data transfer is not interrupted
  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();
};

void Dac::enable_SDO()
{
  byte commandByte3 = B00001000;
  byte commandByte2 = B00000000;
  byte commandByte1 = B00000000;
  byte commandByte0 = B00000010;
  digitalWrite(SYNC_PIN,LOW); 
  delay(10);
  SPI.beginTransaction(this->settingsA);
  for (int i = 1; i < NUM_DEVICES; i++)
  {
    SPI.transfer(commandByte3);
    SPI.transfer(commandByte2);
    SPI.transfer(commandByte1);
    SPI.transfer(commandByte0);
    delay(10); //arbitrary delay to ensure data transfer is not interrupted
  }
  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();
}

/* NON-FUNCTIONAL, DO NOT USE W/O FIXING
byte Dac::SDO_status()
{
  byte commandByte3 = B00011000;
  byte commandByte2 = B00000000;
  byte commandByte1 = B00000000;
  byte commandByte0 = B00000010;
  digitalWrite(SYNC_PIN,LOW); 
  delay(3);
  SPI.beginTransaction(this->settingsA);
  SPI.transfer(commandByte3);
  SPI.transfer(commandByte2);
  SPI.transfer(commandByte1);
  SPI.transfer(commandByte0);
  delay(3); //arbitrary delay to ensure data transfer is not interrupted
  SPI.transfer(B11111111);
  SPI.transfer(B11111111);
  SPI.transfer(B11111111);
  byte SDOstat = SPI.transfer(B11111111);
  delay(3); //arbitrary delay to ensure data transfer is not interrupted
  digitalWrite(SYNC_PIN,HIGH);
  SPI.endTransaction();
  return SDOstat;
}*/

int Dac::current_float_to_int(float f)
{
  int DACint = 0;
  float currentVal = f * MAX_DAC_CODE / MAX_CURRENT;
  //Serial.print("currentVal is: ");
  //Serial.println(currentVal);
  DACint = (int)round(currentVal);
  //Serial.print("DACint is: ");
  //Serial.println(DACint);
  return DACint;
}

// HOPEFULLY will soon be deprecated
//void Dac::set_channel_current(int channel, float f)
//{
//  int DACSetInt = current_float_to_int(f);
//  //make sure we don't exceed limits
//  if(f <= MAX_CURRENT)
//  {
//    digital_write(channel, DACSetInt);
//    delay(10);
//  }
//}

void Dac::set_all_current(int channel, float f)
{
  int DACSetInt = current_float_to_int(f);
  
  digitalWrite(SYNC_PIN,LOW);
  SPI.beginTransaction(this->settingsA);

  for (int i = 0; i < NUM_DEVICES; i++)
  {
    //Serial.print(i);Serial.println("th TRANSFER:");
    send_write_with_update(CHANNEL_ALL,DACSetInt);
  }
  
  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();
}

void Dac::set_device_current(int device, float f)
{
  int DACSetInt = current_float_to_int(f);
  
  digitalWrite(SYNC_PIN,LOW); 
  delay(20);
  SPI.beginTransaction(this->settingsA);

  delay(20);
  send_write_with_update(CHANNEL_ALL,DACSetInt);

  for (int i = 0; i < device; i++)
  {
    delay(20);
    send_nop();
  }
  
  delay(20); //arbitrary delay to ensure data transfer is not interrupted
  digitalWrite(SYNC_PIN,HIGH); 
  SPI.endTransaction();
}

