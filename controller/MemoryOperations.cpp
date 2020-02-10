#include <Arduino.h>
#include <EEPROM.h>

/*
EEPROM organization:
  address:
    A[8]    = 0: MEASUREMENTS   |   1: CONFIG
    A[7:4]  = device   (0-15)   |   device  (0-15)
    A[3:2]  = channel  (0-3)    |   channel (0-3)
    A[1]    = 0: LED, 1:  PD    |   0: ID, 1: DAC
    A[0]    = 0: LSB, 1: MSB    |   0: (LSB), 1: (MSB)
*/

uint16_t get_device_id(int dv, int ch)
{
  // 1 dv dv dv dv ch ch 0 0
  int a = 0x100 + ((dv << 4) & 0xF0) + ((ch << 2) & 0x0C);
  uint16_t id = EEPROM[a];
  id |= (EEPROM[a+1] << 8);
  return id;
}

void set_device_id(int dv, int ch, uint16_t id)
{
  // 1 dv dv dv dv ch ch 0 0
  int a = 0x100 + ((dv << 4) & 0xF0) + ((ch << 2) & 0x0C);
  EEPROM[a] = id & 0xFF;
  EEPROM[a+1] = (id >> 8) & 0xFF;
}

void store_dac_int(int dv, int ch, int dacInt)
{
  // ch,dv DAC LSB = 1 dv dv dv dv ch ch 1 0
  // ch,dv DAC MSB = 1 dv dv dv dv ch ch 1 1
  int a = 0x102 + ((dv << 4) & 0xF0) + ((ch << 2) & 0x0C);
  EEPROM[a] = dacInt & 0xFF;
  EEPROM[a+1] = (dacInt >> 8) & 0xFF;
}

void store_led_meas(int dv, int ch, int v_led)
{
  // ch,dv LED LSB = 0 ch ch ch ch dv dv 0 0
  // ch,dv LED MSB = 0 ch ch ch ch dv dv 0 1
  byte a = ((dv << 4) & 0xF0) + ((ch << 2) & 0x0C);
  EEPROM[a] = v_led & 0xFF;
  EEPROM[a+1] = (v_led >> 8) & 0xFF;
}

void store_pd_meas(int dv, int ch, int v_pd)
{
  // ch,dv PD LSB = 0 ch ch ch ch dv dv 1 0
  // ch,dv PD MSB = 0 ch ch ch ch dv dv 1 1
  byte a = ((dv << 4) & 0xF0) + ((ch << 2) & 0x0C) + 0x02;
  EEPROM[a] = v_pd & 0xFF;
  EEPROM[a+1] = (v_pd >> 8) & 0xFF;
}

uint16_t get_dac_int(int dv, int ch)
{
  // ch,dv DAC LSB = 1 dv dv dv dv ch ch 1 0
  // ch,dv DAC MSB = 1 dv dv dv dv ch ch 1 1
  int a = 0x102 + ((dv << 4) & 0xF0) + ((ch << 2) & 0x0C);
  uint16_t dacInt = EEPROM[a];
  dacInt |= (EEPROM[a+1] << 8);
  return dacInt;
}

uint16_t get_led_meas(int dv, int ch)
{
  // ch,dv LED LSB = 0 ch ch ch ch dv dv 0 0
  // ch,dv LED MSB = 0 ch ch ch ch dv dv 0 1
  byte a = ((dv << 4) & 0xF0) + ((ch << 2) & 0x0C);
  uint16_t v_led = EEPROM[a];
  v_led |= (EEPROM[a+1] << 8);
  return v_led;
}

uint16_t get_pd_meas(int dv, int ch)
{
  // ch,dv PD LSB = 0 ch ch ch ch dv dv 1 0
  // ch,dv PD MSB = 0 ch ch ch ch dv dv 1 1
  byte a = ((dv << 4) & 0xF0) + ((ch << 2) & 0x0C) + 0x02;
  uint16_t v_pd = EEPROM[a];
  v_pd |= (EEPROM[a+1] << 8);
  return v_pd;
}
