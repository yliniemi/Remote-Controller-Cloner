#define INPUT_PIN 15
#define OUTPUT_PIN 13

#define BUFFER_SIZE 1024

volatile unsigned long times[BUFFER_SIZE];
volatile unsigned long counter = 0;
volatile unsigned long lastChange = 0;

void IRAM_ATTR inputChanged()
{
  times[counter] = micros();
  lastChange = micros();
  if (counter < BUFFER_SIZE - 1) counter++;
}

void setup()
{
  Serial.begin(115200);
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, 0);
  attachInterrupt(INPUT_PIN, inputChanged, CHANGE);
  delay(5000);
}

void loop()
{
  static unsigned long anotherCounter = 0;
  static unsigned long sinceLastChange = 0;
  static unsigned long counterCopy = 0;
  sinceLastChange = micros() - lastChange;
  if (sinceLastChange > 100000000) sinceLastChange = 0;
  counterCopy = counter;
  if (counter < BUFFER_SIZE - 1 && counterCopy >= 10 && sinceLastChange > 50000)
  {
    anotherCounter++;
    for (int i = 1; i < counterCopy; i++)
    {
        Serial.print(times[i] - times[i - 1]);
        Serial.print(", ");
        if (i % 10 == 0) Serial.println();
    }
    Serial.println();
    Serial.println(String("Counter ") + counterCopy + ", Second counter " + anotherCounter +  ", delay " + sinceLastChange);
    Serial.println();
    Serial.println();
    counter = 0;
    lastChange = micros();
  }
  else if (counterCopy >= BUFFER_SIZE - 1 && micros() - lastChange > 60000)
  {
    counter = 0;
    lastChange = micros();
  }
  delay(1);
}
