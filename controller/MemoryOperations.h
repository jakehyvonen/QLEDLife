#include <Arduino.h>
#include <EEPROM.h>

uint16_t get_device_id(int);
void set_device_id(int,uint16_t);
void store_dac_int(int,int,int);
void store_led_meas(int,int,int);
void store_pd_meas(int,int,int);
uint16_t get_dac_int(int,int);
uint16_t get_led_meas(int,int);
uint16_t get_pd_meas(int,int);
