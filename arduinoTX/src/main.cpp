#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial lora(2, 3);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  lora.begin(115200);
  lora.print("AT+RESET\r\n");
  delay(20);
  lora.print("AT+ADDRESS=101\r\n");
  delay(20);
  lora.print("AT+NETWORKID=5\r\n");
  // delay(20);
  // lora.print("AT+PARAMETER=10,7,1,7\r\n");
}

void loop()
{
  // put your main code here, to run repeatedly:
  lora.print("AT+SEND=0,5,HELLO\r\n");
  delay(5000);
  lora.print("AT+SEND=0,5,WORLD\r\n");
  delay(5000);
}