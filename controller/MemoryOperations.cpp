#include <Arduino.h>
#include <EEPROM.h>

/*
EEPROM organization:
  address:
    A[8]    = 0: MEASUREMENTS   |   1: CONFIG
    A[7:4]  = channel (0-15)    |   channel (0-15)
    A[3:2]  = device  (0-3)     |   
    A[1]    = 0: LED, 1:  PD    |   10: data valid
    A[0]    = 0: LSB, 1: MSB    |   00: id (LSB), 01: id (MSB)
*/

uint16_t get_device_id(uint16_t channel)
{
  // 1 ch ch ch ch 0 0 0 0
  int a = 0x100 + ((channel << 4) & 0xF0);
  uint16_t id = EEPROM[a];
  id |= (EEPROM[a+1] << 8);
}

void set_device_id(int channel, uint16_t id)
{
  // 1 ch ch ch ch 0 0 0 0
  int a = 0x100 + ((channel << 4) & 0xF0);
  EEPROM[a] = id & 0xFF;
  EEPROM[a+1] = (id >> 8) & 0xFF;
}

void set_data_valid(int channel)
{
  // 1 ch ch ch ch 0 0 0 1
  int a = 0x102 + ((channel << 4) & 0xF0);
  EEPROM[a] = 1;
}

void clear_data_valid(int channel)
{
  // 1 ch ch ch ch 0 0 0 1
  int a = 0x102 + ((channel << 4) & 0xF0);
  EEPROM[a] = 0;
}

bool is_data_valid(int channel)
{
  // 1 ch ch ch ch 0 0 0 1
  int a = 0x101 + ((channel << 4) & 0xF0);
  return EEPROM[a];
}

void store_led_meas(int channel, int device, int v_led)
{
  // ch,dv LED LSB = 0 ch ch ch ch dv dv 0 0
  // ch,dv LED MSB = 0 ch ch ch ch dv dv 0 1
  byte a = ((channel << 4) & 0xF0) + ((device << 2) & 0x0C);
  EEPROM[a] = v_led & 0xFF;
  EEPROM[a+1] = (v_led >> 8) & 0xFF;
}

void store_pd_meas(int channel, int device, int v_pd)
{
  // ch,dv PD LSB = 0 ch ch ch ch dv dv 1 0
  // ch,dv PD MSB = 0 ch ch ch ch dv dv 1 1
  byte a = ((channel << 4) & 0xF0) + ((device << 2) & 0x0C) + 0x02;
  EEPROM[a] = v_pd & 0xFF;
  EEPROM[a+1] = (v_pd >> 8) & 0xFF;
}

int get_led_meas(int channel, int device)
{
  // ch,dv LED LSB = 0 ch ch ch ch dv dv 0 0
  // ch,dv LED MSB = 0 ch ch ch ch dv dv 0 1
  byte a = ((channel << 4) & 0xF0) + ((device << 2) & 0x0C);
  int v_led = EEPROM[a];
  v_led |= (EEPROM[a+1] << 8);
  return v_led;
}

int get_pd_meas(int channel, int device)
{
  // ch,dv PD LSB = 0 ch ch ch ch dv dv 1 0
  // ch,dv PD MSB = 0 ch ch ch ch dv dv 1 1
  byte a = ((channel << 4) & 0xF0) + ((device << 2) & 0x0C) + 0x02;
  int v_pd = EEPROM[a];
  v_pd |= (EEPROM[a+1] << 8);
  return v_pd;
}
