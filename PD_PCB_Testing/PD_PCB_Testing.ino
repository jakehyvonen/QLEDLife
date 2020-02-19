#include <Wire.h>
#include <Adafruit_ADS1015.h>

//Mux control pins
int s0 = 4;
int s1 = 5;
int s2 = 6;
int s3 = 7;
Adafruit_ADS1115 ads1115;

void setup(void)
{
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT); 
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  Serial.begin(9600);
  Serial.println("Hello!");
  ads1115.setGain(GAIN_TWOTHIRDS);
  //Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV)"); 
  //ads1115.setGain(GAIN_SIXTEEN); // 16x gain  +/- 0.256V  1 bit = 0.0078125 mV = 7.8 uV
  //Serial.println("ADC Range: +/- 0.256V (1 bit = 0.125mV)");
  ads1115.begin();
}

void loop(void)
{
  /*
  switchMuxChannel(0);
  delay(1000);
  float result = ads1115.readADC_SingleEnded(0);
  Serial.print("For channel: 0, ");
  Serial.print("Result bit = "); Serial.println(result);
  Serial.print("so, current = "); Serial.print(result*0.000125/120000.0,10);Serial.println(" A");
  delay(1000);
  switchMuxChannel(3);
  delay(1000);
  result = ads1115.readADC_SingleEnded(0);
  Serial.print("For channel: 3, "); 
  Serial.print("Result bit = "); Serial.println(result);
  Serial.print("so, voltage = "); Serial.print(result*0.0078125/68.0,5);Serial.println(" mV");
  delay(1000);
  */
  for (int i = 0; i <= 3; i++)
  {
    float result;
    switchMuxChannel(i);
    delay(111);
    result = ads1115.readADC_SingleEnded(3);
    Serial.print(" chan: "); Serial.print(i);
    Serial.print(" bit = "); Serial.print(result);
    //Serial.print("so, current = "); Serial.print(result*0.0001875/120000.0,8);Serial.println(" A");
    delay(111);
  }
  Serial.println();
}

void switchMuxChannel(int channel) 
{
  int controlPin[] = {s0, s1, s2, s3};
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
    {1, 1, 1, 1} //channel 15
  };

  //loop through the 4 sig
  for (int j = 0; j < 4; j ++)
  {
    digitalWrite(controlPin[j], muxChannel[channel][j]);
    //Serial.print(muxChannel[channel][j]);
    //Serial.println();
  }

}
