#include <Arduino.h>
#include "MUX.h"

Mux::Mux(int numCh, int* pin) : nCh(numCh)
{
  this->nPins = 0;
  if (nCh >= 16)
  {
      controlPin[3] = pin[3];
      nPins++;
  }
  if (nCh >= 8)
  {
      controlPin[2] = pin[2];
      nPins++;
  }
  if (nCh >= 4)
  {
      controlPin[1] = pin[1];
      nPins++;
  }
  if (nCh >= 2)
  {
      controlPin[0] = pin[0];
      nPins++;
  }
  for (int i = 0; i < nPins; i++)
  {
    pinMode(controlPin[i],OUTPUT);
    digitalWrite(controlPin[i],LOW);
  }
  this->channel = 0;
}


void Mux::switch_channel(int channel) 
{
  if (channel < nCh)
  {
    //Serial.print("switchMuxChannel received this int: "); Serial.println(channel);
    int muxChannel[16][4] = {
      {0, 0, 0, 0}, //channel 0
      {1, 0, 0, 0}, //channel 1
      {0, 1, 0, 0}, //channel 2
      {1, 1, 0, 0}, //channel 3
      {0, 0, 1, 0}, //channel 4
      {1, 0, 1, 0}, //channel 5
      {0, 1, 1, 0}, //channel 6
      {1, 1, 1, 0}, //channel 7
      {0, 0, 0, 1}, //channel 8
      {1, 0, 0, 1}, //channel 9
      {0, 1, 0, 1}, //channel 10
      {1, 1, 0, 1}, //channel 11
      {0, 0, 1, 1}, //channel 12
      {1, 0, 1, 1}, //channel 13
      {0, 1, 1, 1}, //channel 14
      {1, 1, 1, 1}  //channel 15
    };

    //Serial.print("switching to ch ");
    //Serial.println(channel);
  
    //loop through the 4 sig
    for (int j = 0; j < nPins; j++)
    {
      digitalWrite(controlPin[j], muxChannel[channel][j]);
      //Serial.print(muxChannel[channel][j]);
      //Serial.println();
    }
  
    this->channel = channel;
  }
}

int Mux::get_channel()
{
  return this->channel;
}

void Mux::print_control_pins()
{
  Serial.println(String(controlPin[0]));
  Serial.println(String(controlPin[1]));
  Serial.println(String(controlPin[2]));
  Serial.println(String(controlPin[3]));
}

