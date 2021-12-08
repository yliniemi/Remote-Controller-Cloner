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


void Transmitter::sendRaw(const std::vector<unsigned int> timings)
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
