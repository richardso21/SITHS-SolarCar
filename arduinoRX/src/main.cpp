#include <Arduino.h>
int led = 13;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, INPUT);
}

int ranVal1;
int ranVal2;
int ranVal3;

void loop()
{
  ranVal1 = random(10, 20);
  ranVal2 = random(100, 150);
  ranVal3 = random(5);

  String res = String(ranVal1) + ";" + String(ranVal2) + ";" + String(ranVal3);
  // put your main code here, to run repeatedly:
  Serial.println(res);
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
    delay(250);
  }
}