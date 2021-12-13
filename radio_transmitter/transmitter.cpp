#include <Arduino.h>
#include "transmitter.h"

Transmitter::Transmitter(int setPin, int setRestingState)
{
  outputPin = setPin;
  restingState = setRestingState;
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, restingState);
}
  
Transmitter::Transmitter(int setPin)
{
  Transmitter(setPin, 0);
}


void Transmitter::sendRaw(std::vector<unsigned int> timings, unsigned int repeat)
{
  unsigned long int previousMicros = micros();
  unsigned long int newWrite;
  unsigned long int previousWrite;
  unsigned long int delayMicros;

  std::vector<int> delays;
  int pinStatus = restingState;
  
  // these lines are here to make the timing as precice as possible
  /*
  digitalWrite(outputPin, pinStatus);
  newWrite = micros();
  previousWrite = newWrite;
  delayMicros = previousMicros + delayBeforeMessage - micros();
  if (delayMicros < 10000000)
  {
    delayMicroseconds[delayMicros];
  }
  */
  previousMicros = micros();
  for (int i = 0; i < repeat; i++)
  {
    pinStatus = !restingState;            // I'm doing this so that the first timing tells you how long you wait at zero before starting transmission
    // Now we start sending the actual message
    for (auto timing : timings)
    {
      pinStatus = !pinStatus;
      digitalWrite(outputPin, pinStatus);
      newWrite = micros();
      delays.push_back(newWrite - previousWrite);
      previousWrite = newWrite;
      delayMicros = previousMicros + timing - micros();
      if (delayMicros < 10000000)
      {
        delayMicroseconds(delayMicros);
      }
      //previousMicros = micros();
      previousMicros = previousMicros + timing;
    }
  }
  // that is a return to normality
  pinStatus = restingState;
  digitalWrite(outputPin, pinStatus);
  Serial.println();
  Serial.println("Sent this data:");
  int i = 0;
  for (auto timing : timings)
  {
    if ((i < 2) || (i % 2 == 1)) Serial.print(String(timing) + ", ");
    else Serial.println(timing);
    i++;
  }
}

bool Transmitter::bitUint8(uint8_t data, int bitIndex)
{
  static const uint8_t bitLocation[] = {
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001,
  };
  // Serial.print(data & bitLocation[bitIndex]);
  Serial.print(((data & bitLocation[bitIndex]) != 0));
  // Serial.print(", ");
  if ((data & bitLocation[bitIndex]) == 0) return false;
  else return true;
}

void Transmitter::sendProtocol(Protocol protocol, std::vector<uint8_t> message, unsigned int length, unsigned int repeat)
{
  std::vector<unsigned int> rawMessage;
  rawMessage.insert(rawMessage.end(), protocol.preamble.begin(), protocol.preamble.end());
  length = length % 8;     // I'm doing the modulo so that 0 and 8 are the same. Incidentally you could put the leght of the whole signal here and it would work.
  if (length == 0) length = 8;
  int lastBit = 8;
  Serial.print("Message is: ");
  for (int i = 0; i < message.size(); i++)
  {
    if (i == message.size() - 1) lastBit = length;
    {
      for (int j = 0; j < lastBit; j++)
      {
        if (bitUint8(message[i], j)) rawMessage.insert(rawMessage.end(), protocol.one.begin(), protocol.one.end());
        else rawMessage.insert(rawMessage.end(), protocol.zero.begin(), protocol.zero.end());
      }
    }
  }
  Serial.println();
  Serial.println();
  Transmitter::sendRaw(rawMessage, repeat);
}
