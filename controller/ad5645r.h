#ifndef AD5645R_H
#define AD5645R_H

#define AD5645R_DAC_CHANNEL_A    (0x0)
#define AD5645R_DAC_CHANNEL_B    (0x1)
#define AD5645R_DAC_CHANNEL_C    (0x2)
#define AD5645R_DAC_CHANNEL_D    (0x3)
#define AD5645R_DAC_CHANNEL_ALL  (0x7)

#define AD5645R_CMD_WRITE_UPDATE_N   (0x3 << 3) 

class AD5645R
{
public:
  AD5645R(uint8_t i2cAddress);
  void write_update_n(uint8_t ch, uint16_t DacInt);
private:
  uint8_t m_i2cAddress;
};


#endif
