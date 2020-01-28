#include <Arduino.h>
#include <Wire.h>
#include "DAC.h"
#include "defines.h"
#include "MemoryOperations.h"

Dac::Dac(int maxCode, float maxCurrent, int numDevices) : m_maxCode(maxCode), m_maxCurrent(maxCurrent), m_numDevices(numDevices) {};

static void i2cwrite(uint8_t i2cAddress, uint8_t command, uint16_t data)
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(command);
  Wire.write((uint8_t)(data >> 6));
  Wire.write((uint8_t)((data << 2) & 0xFF));
  Wire.endTransmission();
}

void Dac::begin()
{
  // nothing to initialize?
};

int Dac::current_float_to_int(float f)
{
  float currentVal = f * m_maxCode / m_maxCurrent;
  int DACint = (int)round(currentVal);
  return DACint;
}

float Dac::current_int_to_float(int i)
{
  float currentFloat = ((float)i / (float)m_maxCode) * m_maxCurrent;
  return currentFloat;
}

int Dac::set_all_current(float f)
{
  if (f > m_maxCurrent)
    return -1;
    
  int DacSetInt = current_float_to_int(f);

  if (DacSetInt > m_maxCode)
    return -1;

  //TODO - error check here
  uint8_t commandByte = AD5645R_CMD_WRITE_UPDATE_N | AD5645R_DAC_CHANNEL_ALL;
  i2cwrite(AD5645R_BROADCAST_ADDR,commandByte,DacSetInt);

  return 0;
}

int Dac::set_current(int device, int channel, float f)
{
  if (channel < 0 || channel > 3)
    return -1;
  if (device >= m_numDevices) 
    return -1;
  if (f > m_maxCurrent)
    return -1;
  
  int DacSetInt = current_float_to_int(f);

  if (DacSetInt > m_maxCode)
    return -1;

  //TODO - error check here
  uint8_t commandByte = AD5645R_CMD_WRITE_UPDATE_N | channel;
  i2cwrite(m_devices[device],commandByte,DacSetInt);

  store_dac_int(device,channel,DacSetInt);

  return 0;
}

/* ######################################################
*                         TEST fn
*         These functions work. Use to debug.
*  ######################################################
*/
