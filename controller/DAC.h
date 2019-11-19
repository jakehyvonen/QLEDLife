#ifndef DAC_H
#define DAC_H

#include <SPI.H>

class Dac
{
  public:
    Dac(int maxCode, float maxCurrent, int numDev, int syncPin);
    void begin();
    void enable_SDO();
    void read_register(byte reg, byte* buf);
    void set_all_current(int channel, float f, bool upd);
    void set_device_current(int device, float f);
    void test_write();
    void test_enable_SDO();
    void test_clr();

  private:
    const int MAX_DAC_CODE = 16383;
    const float MAX_CURRENT = 10.0;
    const int NUM_DEVICES = 1;
    const int SYNC_PIN = 9;
    SPISettings settingsA;
    int current_float_to_int(float f);
    void digital_write(int address, int value);
    void send_write_buf(unsigned int address, unsigned int value, bool upd);
    void send_nop(byte* buf);
};

#endif
