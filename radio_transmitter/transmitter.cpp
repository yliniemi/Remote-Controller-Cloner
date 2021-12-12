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


void Transmitter::sendRaw(std::vector<unsigned int> timings)
{
  unsigned long int previousMicros = micros();
  unsigned long int newWrite;
  unsigned long int previousWrite;
  unsigned long int delayMicros;

  std::vector<int> delays;
  
  // these lines are here to make the timing as precice as possible
  int pinStatus = restingState;
  digitalWrite(outputPin, pinStatus);
  newWrite = micros();
  previousWrite = newWrite;
  delayMicros = previousMicros + delayBeforeMessage - micros();
  if (delayMicros < 10000000)
  {
    delayMicroseconds[delayMicros];
  }
  previousMicros = previousMicros + delayBeforeMessage;
  
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
  pinStatus = restingState;
  digitalWrite(outputPin, pinStatus);
  delays.push_back(micros() - previousWrite);
  // delay(20);
  // Serial.println("Sent some data over the radio waves");
  int biggestDelta = 0;
  int biggestDeltaLine = 0;
  /*for (int i = 2; i < length -1; i++)
  {
    if (abs(delayArray[i] - array[i - 1]) > biggestDelta)
    {
      biggestDelta = abs(delayArray[i] - array[i - 1]);
      biggestDeltaLine = i;
    }
  }
  Serial.println(String("The biggest timing error was: ") + biggestDelta + " on line " + biggestDeltaLine);
  */
}

bool Transmitter::bitUint8(uint8_t data, int bit)
{
  return (data & (128 >> bit) != 0);
}

void Transmitter::sendProtocol(Protocol protocol, std::vector<uint8_t> message, unsigned int length, unsigned int repeat)
{
  std::vector<unsigned int> rawMessage;
  rawMessage.insert(rawMessage.end(), protocol.preamble.begin(), protocol.preamble.end());

  int lastBit = 8;
  for (int i = 0; i < message.size(); i++)
  {
    if (i == (message.size() - 1)) lastBit = length;
    {
      for (int j = 0; j < lastBit; j++)
      {
        if (bitUint8(message[i], j)) rawMessage.insert(rawMessage.end(), protocol.one.begin(), protocol.one.end());
        else rawMessage.insert(rawMessage.end(), protocol.zero.begin(), protocol.zero.end());
      }
    }
  }
}
