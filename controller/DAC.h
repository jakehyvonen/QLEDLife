#ifndef DAC_H
#define DAC_H

#include <SPI.H>

class Dac
{
  public:
  Dac(int maxCode, float maxCurrent, int numDev, int syncPin);
  void begin();
  void enable_SDO();
  byte SDO_status();
  void set_all_current(int channel, float f);
  void set_device_current(int device, float f);

  private:
  const int MAX_DAC_CODE = 16383;
  const float MAX_CURRENT = 10.0;
  const int NUM_DEVICES = 1;
  const int SYNC_PIN = 9;
  SPISettings settingsA;
  int current_float_to_int(float f);
  void digital_write(int address, int value);
  void send_write_with_update(unsigned int address, unsigned int value);
  void send_nop();
};

#endif
