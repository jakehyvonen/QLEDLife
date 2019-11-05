#include <Arduino.h>
#include <EEPROM.h>

byte get_device_id(int);
void set_device_id(int,uint16_t);
void set_data_valid(int);
void clear_data_valid(int);
bool is_data_valid(int);
void store_led_meas(int,int,int);
void store_pd_meas(int,int,int);
int get_led_meas(int,int);
int get_pd_meas(int,int);
