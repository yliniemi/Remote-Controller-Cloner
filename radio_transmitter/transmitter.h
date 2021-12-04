#ifndef TRANSMITTER_H
#define TRANSMITTER_H

class Transmitter
{
  public:
  int outputPin;
  int restingState;
  unsigned int delayBeforeMessage = 10000;
  
  Transmitter(int setPin, int setRestingState);
  
  Transmitter(int setPin);
  
  void sendRaw(unsigned int *array, int length);
};

#endif
