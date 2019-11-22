#include <SPI.h>
#include <Wire.h>
#include <WString.h>
#include <Adafruit_ADS1015.h> //ADS1115 library
#include "DAC.h"
#include "MUX.h"
#include "MemoryOperations.h"
#include "defines.h"

#define NUM_DEVICES 3
#define MSG_METHOD_SUCCESS 0

const int MUX16CHSELS[] = {DPIN2, DPIN3, DPIN4, DPIN5};
const int MUX4CHSELS[] = {DPIN6, DPIN7};

Adafruit_ADS1115 ADS1115(0x48);
Mux MUX16CH(16, &MUX16CHSELS[0]);
Mux MUX4CH(4, MUX4CHSELS);
Dac DAC(MAX14BIT, MAX10MA, NUM_DEVICES, DPIN8);

float VdevADCGainCF = (0.0001875) * (27.0 / 12.0); //arduino ADC (10 bit)*(120+150kohm)/(120 kohm)
//float IphADCGainCF = 0.0001875 / 120000.0; //units are Amps
float IphADCGainCF = 0.0001875; //units are Volts

int chSel = CHANNEL_A;
int devSel = DEVICE_0;

int measurementPeriod = 1; // in seconds
int timerCount = 0;
bool timerFlag = 0;

float sweepValue = 0;
bool sweepUp = 1;
bool sweeping = 0;

/* #####################################################################
                   MAIN
   #####################################################################
*/

void setup() {
  noInterrupts();

  Serial.begin(9600);

  DAC.begin();
  delay(100);
  
  ADS1115.setGain(GAIN_TWOTHIRDS);
  ADS1115.begin();

  //Serial.println("ready");

  interrupts();
}

void loop() {
  if (timerFlag)
  {
    /* clear flag */
    timerFlag = 0;

    int result;

    devSel = (devSel + 1) % NUM_DEVICES;
    clear_data_valid(devSel);
    MUX16CH.switch_channel(devSel);
    for (chSel = 0; chSel < 4; chSel++)
    {
      MUX4CH.switch_channel(devSel);
      delay(5);
      result = ADS1115.readADC_SingleEnded(0);    // read LED voltage
      store_led_meas(devSel, chSel, result);
      result = ADS1115.readADC_SingleEnded(1);    // read PD voltage
      store_pd_meas(devSel, chSel, result);
      /*VdevFloat = VdevBit * VdevADCGainCF;
        Serial.print(String(get_device_id(chSel)));
        Serial.print(",");
        Serial.print(get_device_name(devSel));
        Serial.print(" PDV: ");
        Serial.println(VdevFloat);
        IphBit = ADS1115.readADC_SingleEnded(1);
        IphFloat = IphBit*IphADCGainCF;
        Serial.print(String(get_device_id(chSel)));
        Serial.print(",");
        Serial.print(get_device_name(devSel));
        Serial.print(" LEDV: ");
        Serial.println(String(IphBit));
        Serial.println(IphFloat);*/
    }
    set_data_valid(devSel);
  }

  //Serial Input Section -- handles commands received from software
  String command = "";  //Used to store the latest received command
  int serialResult = 0; //return value for reading operation method on serial in put buffer
  serialResult = readSerialInputCommand(&command);

  if (serialResult == MSG_METHOD_SUCCESS)
  {
    if (command.substring(0, 5) == "setID")
    {
      String parseString = command.substring(5, 7); // get the channel number as a string
      int channelInt = parseString.toInt();        // then convert it to an int
      parseString = command;
      parseString.remove(0, 7);
      uint16_t DBIDInt = parseString.toInt();
      set_device_id(channelInt, DBIDInt);
    }
    else if (command.substring(0, 6) == "setAll")
    {
      String parseString = command.substring(6, 10); // get the I value as a string
      float currentFloat = parseString.toFloat();   // then convert it to an float
      DAC.set_all_current(CHANNEL_ALL, currentFloat);
    }
    else if (command.substring(0, 6) == "setDev")
    {
      String parseString = command.substring(6, 8); // get the device number as a string
      int device = parseString.toInt();
      parseString = command.substring(8, 9);
      int ch = channel_name_to_number(parseString.charAt(0));
      parseString = command.substring(9, 13); // get the I value as a string
      float currentFloat = parseString.toFloat();   // then convert it to an float
      //Serial.print("Setting device ");Serial.print(device);Serial.print(" to ");Serial.println(currentFloat);
      DAC.set_device_current(device, ch, currentFloat);
    }
    else if (command.substring(0, 6) == "getLed")
    {
      String parseString = command.substring(6, 8); // get channel as string
      int dev = parseString.toInt();
      parseString = command.substring(8, 9);
      int ch = channel_name_to_number(parseString.charAt(0));
      if (is_data_valid(dev))
      {
        int v_led_int = get_led_meas(ch, dev);
        Serial.println(ch);
        Serial.println(dev);
        Serial.println(v_led_int);
      }
    }
    else if (command.substring(0,4) == "read")
    {
      String parseString = command.substring(5, 7); // get device as int
      int dv = parseString.toInt();
      parseString = command.substring(7, 8);        // get channel as int
      int ch = channel_name_to_number(parseString.charAt(0));
      int current = DAC.read_device_current(dv,ch);
    }
    else if (command.substring(0, 4) == "stop")
    {
      stop_timer();
    }
    else if (command.substring(0, 5) == "start")
    {
      start_timer();
    }
    else if (command.substring(0, 4) == "ping")
    {
      Serial.print("pong");
    }
    else if (command.substring(0, 5) == "sweep")
    {
      sweeping ? stop_sweep() : start_sweep();
    }
    else if (command.substring(0, 6) == "_write")
    {
      DAC.test_write();
    }
    else if (command.substring(0, 4) == "_sdo")
    {
      DAC.test_enable_SDO();
    }
    else if (command.substring(0, 4) == "_clr")
    {
      DAC.test_clr();
    }
    else if (command.substring(0,5) == "_read")
    {
      String parseString = command.substring(5, 7); // get register as string
      int reg = parseString.toInt();
      parseString = command.substring(7, 8);        // get channel as int
      int ch = channel_name_to_number(parseString.charAt(0));
      byte buf[4*NUM_DEVICES];
      DAC.test_read_register(reg,ch,&buf[0]);
    }
  }
}

ISR(TIMER1_OVF_vect)
{
  /* increment "count" (elapsed seconds) */
  timerCount++;
  if (timerCount == measurementPeriod)
  {
    /* flag the main loop & reset */
    timerFlag = 1;
    timerCount = 0;
  }
  TCNT1 = 3037;
}

ISR(TIMER2_OVF_vect)
{
  TCNT2 = 0;
  DAC.set_all_current(CHANNEL_ALL, sweepValue);
  if (sweepUp)
  {
    if (sweepValue < 10.0)
    {
      sweepValue += 0.01;
    }
    else
    {
      sweepUp = 0;
      sweepValue -= 0.01;
    }
  }
  else
  {
    if (sweepValue > 0.0)
    {
      sweepValue -= 0.01;
    }
    else
    {
      sweepUp = 1;
      sweepValue += 0.01;
    }

  }
}

/* #####################################################################
                    HELPER Fns
   #####################################################################
*/

char channel_number_to_name(int channel)
{
  return char(channel+65);
}

int channel_name_to_number(char channel)
{
  if (channel >= 65 && channel <= 68)
  {
    return (channel - 65);
  }
  return CHANNEL_ALL;
}

int readSerialInputCommand(String *command)
{
  int operationStatus = MSG_METHOD_SUCCESS;//Default return is MSG_METHOD_SUCCESS reading data from com buffer.

  //check if serial data is available for reading
  if (Serial.available())
  {
    char serialInByte;//temporary variable to hold the last serial input buffer character

    while (serialInByte != '#' && Serial.available()) //until '#' comes up or no serial data is available anymore
    {
      serialInByte = Serial.read();
      *command = *command + serialInByte;//Add last read serial input buffer byte to *command pointer
      delay(10);
    }
  }
  return operationStatus;
}

void start_timer()
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 3037;
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << TOIE1);
  interrupts();
}


void stop_timer()
{
  noInterrupts();
  TCNT1 = 0;
  TCCR1B &= ~(1 << CS12);
  TIMSK1 &= ~(1 << TOIE1);
  interrupts();
}

void start_sweep()
{
  noInterrupts();
  Serial.println("Starting sweep...");
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  TCCR2B |= ((1 << CS12) + 1);    // 1024 prescaler
  TIMSK2 |= (1 << TOIE1);
  interrupts();
  sweeping = 1;
}

void stop_sweep()
{
  noInterrupts();
  Serial.println("Stopping sweep...");
  sweeping = 0;
  TCNT2 = 0;
  TCCR2B &= ~((1 << CS12) + 1);    // 1024 prescaler
  TIMSK2 &= ~(1 << TOIE1);
  DAC.set_all_current(CHANNEL_ALL, 0);
  interrupts();
}



