#ifndef DAC_H
#define DAC_H

#include <Wire.h>

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

class Dac
{
  public:
    Dac(int maxCode, float maxCurrent, int numDevices);
    void begin();
    int set_all_current(float f);
    int set_current(int device, int channel, float f);

  private:
    const int m_maxCode = ((1 << 14) - 1);   // 14-bits
    const float m_maxCurrent = 10.0;
    const int m_numDevices = 1;
    const uint8_t m_devices[9] = {
      0x10,0x12,0x13,
      0x18,0x1A,0x1B,
      0x1C,0x1E,0x1F
    };
    int current_float_to_int(float f);
    float current_int_to_float(int i);
};

#endif
