#include "main.hpp"
LoraSerial lora(8, 9);

void setup()
{
  Serial.begin(9600);
  lora.begin(38400);
}

void loop()
{
  if (lora.available())
    Serial.println(lora.parseData());
}
