#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial lora(2, 3);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(9600);
  lora.begin(115200);
  lora.print("AT+RESET\r\n");
  delay(20);
  lora.print("AT+ADDRESS=102\r\n");
  delay(20);
  lora.print("AT+NETWORKID=5\r\n");
  // delay(20);
  // lora.print("AT+PARAMETER=10,7,1,7\r\n");
  lcd.init();
  lcd.backlight();
}

void loop()
{
  String msg;
  while (lora.available())
  {
    if (lora.available())
    {
      msg += String(char(lora.read()));
    }
  }
  if (msg.length() > 0)
  {
    lcd.clear();
    lcd.println(msg);
    Serial.println(msg);
  }
}