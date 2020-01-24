#include <Arduino.h>
#include <Wire.h>
#include "ad5645r.h"

AD5645R::AD5645R(uint8_t i2cAddress) : m_i2cAddress(i2cAddress) {};

static void i2cwrite(uint8_t i2cAddress, uint8_t command, uint16_t data)
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(i2cAddress);
  Wire.write(command);
  Wire.write((uint8_t)(data >> 8));
  Wire.write((uint8_t)(data & 0xFF));
  Wire.endTransmission();
}

void AD5645R::write_update_n(uint8_t ch, uint16_t DacInt)
{
  uint8_t command_byte = AD5645R_CMD_WRITE_UPDATE_N | ch;
  i2cwrite(this->m_i2cAddress,command_byte,DacInt);
}