#ifndef TRANSMITTER_H
#define TRANSMITTER_H

class Transmitter
{
  public:
  int outputPin;
  int restingState;
  unsigned int delayBeforeMessage = 10000;
  
  struct Protocol
  {
    std::vector<unsigned int> preamble;
    std::vector<unsigned int> zero;
    std::vector<unsigned int> one;
  };

  Transmitter(int setPin, int setRestingState);
  
  Transmitter(int setPin);
  
  void sendRaw(const std::vector<unsigned int> timings, unsigned int repeat);
  bool bitUint8(uint8_t data, int bit);
  void sendProtocol(Protocol protocol, std::vector<uint8_t> message, unsigned int length, unsigned int repeat);
};

#endif
