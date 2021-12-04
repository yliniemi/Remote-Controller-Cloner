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


void Transmitter::sendRaw(unsigned int *array, int length)
{
  unsigned long int previousMicros = micros();
  unsigned long int newWrite;
  unsigned long int previousWrite;
  unsigned long int delayMicros;

  unsigned int delayArray[1024];
  unsigned int statusArray[1024];
  
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
  for (int i = 0; i < length - 1; i++)
  {
    pinStatus = !pinStatus;
    digitalWrite(outputPin, pinStatus);
    newWrite = micros();
    delayArray[i] = newWrite - previousWrite;
    previousWrite = newWrite;
    delayMicros = previousMicros + array[i] - micros();
    if (delayMicros < 10000000)
    {
      delayMicroseconds(delayMicros);
    }
    statusArray[i] = pinStatus;
    //previousMicros = micros();
    previousMicros = previousMicros + array[i];
  }
  pinStatus = restingState;
  digitalWrite(outputPin, pinStatus);
  delay(20);
  Serial.println("Sent some data over the radio waves");
  int biggestDelta = 0;
  int biggestDeltaLine = 0;
  for (int i = 2; i < length -1; i++)
  {
    Serial.println(String("Line: ") + i + ", Delay was: " + delayArray[i] + ", it should have been: " + array[i - 1] + ", error: " + abs(delayArray[i] - array[i - 1]));
    if (abs(delayArray[i] - array[i - 1]) > biggestDelta)
    {
      biggestDelta = abs(delayArray[i] - array[i - 1]);
      biggestDeltaLine = i;
    }
  }
  Serial.println(String("The biggest timing error was: ") + biggestDelta + " on line " + biggestDeltaLine);
}
