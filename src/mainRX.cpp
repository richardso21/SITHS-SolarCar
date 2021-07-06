#include "main.hpp"
LoraSerial lora(8, 9);
int pin = 13;

void setup()
{
    Serial.begin(9600);
    lora.begin(9600);
    pinMode(pin, OUTPUT);
}

void loop()
{
    if (lora.available())
    {
        digitalWrite(pin, HIGH);
        Serial.println(lora.parseData());
        digitalWrite(pin, LOW);
    }
}
