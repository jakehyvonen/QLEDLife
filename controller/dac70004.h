#ifndef DAC70004_H
#define DAC70004_H

#define DAC_WRITE_BUF_CMD             0x00
#define DAC_UPDATE_CMD                0x01
#define DAC_WRITE_BUF_UPDATE_ALL_CMD  0x02
#define DAC_WRITE_BUF_UPDATE_N_CMD    0x03
#define DAC_PWR_CMD                   0x04
#define DAC_CLR_CMD                   0x05
#define DAC_LDAC_CMD                  0x06
#define DAC_SWRST_CMD                 0x07
#define DAC_SDO_CMD                   0x08
#define DAC_SHORT_CMD                 0x0A
#define DAC_SWCLR_CMD                 0x0B
#define DAC_STATUS_CMD                0x0D
#define DAC_NOP_CMD                   0x0E

#define DAC_READ                      0x10
#define DAC_WRITE                     0x00

#define DAC_CHANNEL_A                 (0x00 << 4)
#define DAC_CHANNEL_B                 (0x01 << 4)
#define DAC_CHANNEL_C                 (0x02 << 4)
#define DAC_CHANNEL_D                 (0x03 << 4)
#define DAC_CHANNEL_ALL               (0x0F << 4)

#define DAC_PWR_CH_A                  0x01
#define DAC_PWR_CH_B                  0x02
#define DAC_PWR_CH_C                  0x04
#define DAC_PWR_CH_D                  0x08

#define DAC_LDAC_CH_A                 0x01
#define DAC_LDAC_CH_B                 0x02
#define DAC_LDAC_CH_C                 0x04
#define DAC_LDAC_CH_D                 0x08

#define DAC_SHORT_CH_A                0x01
#define DAC_SHORT_CH_B                0x02
#define DAC_SHORT_CH_C                0x04
#define DAC_SHORT_CH_D                0x08

#define DAC_PWR_UP                    0x00
#define DAC_PWR_DN_1K                 0x01
#define DAC_PWR_DN_100K               0x02
#define DAC_PWR_DN_HIZ                0x03

#define DAC_CLR_ZERO                  0x00
#define DAC_CLR_MID                   0x01
#define DAC_CLR_FULL                  0x02

#define DAC_LDAC_PIN                  0x00
#define DAC_LDAC_IGNORE               0x01

#define DAC_SHORT_40MA                0x00
#define DAC_SHORT_30MA                0x01

#define DAC_SDO_EN                    0x02

#endif
