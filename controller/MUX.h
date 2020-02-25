#ifndef MUX_H
#define MUX_H

class Mux
{
  public:
  Mux(int numCh, int* pin);
  void switch_channel(int channel);
  int get_channel();
  void print_control_pins();
  
  private:
  const int nCh;
  int nPins;
  int controlPin[4];
  int channel;
};

#endif
