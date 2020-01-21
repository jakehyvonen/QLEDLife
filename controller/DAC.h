#ifndef DAC_H
#define DAC_H

#include <SPI.H>

class Dac
{
  public:
    Dac(int maxCode, float maxCurrent, int numDev, int syncPin);
    void begin();
    void enable_SDO();
    void set_all_current(float f);
    int set_current(int device, int channel, float f);
    float read_device_current(int device, int channel);
    void test_write();
    void test_enable_SDO();
    void test_clr();
    void test_read_register(byte reg, int ch, byte* buf);

  private:
    const int MAX_DAC_CODE = 16383;
    const float MAX_CURRENT = 10.0;
    const int NUM_DEVICES = 1;
    const int SYNC_PIN = 9;
    SPISettings settingsA;
    int current_float_to_int(float f);
    float current_int_to_float(int i);
    int transmit(byte *tx_buf, byte *rx_buf, int len);
    void send_nop(byte* buf);
};

#endif
