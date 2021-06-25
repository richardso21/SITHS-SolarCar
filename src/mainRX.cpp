#include "main.hpp"
LiquidCrystal_I2C lcd(0x27, 20, 4);
AltSoftSerial loraSerial;

void setup()
{
  Serial.begin(9600);
  loraSerial.begin(9600);
  lcd.init();
  lcd.backlight();
}

String msgRX;

void loop()
{
  if (loraSerial.available())
  {
    msgRX = loraSerial.readString();
    lcd.clear();
    lcd.println(msgRX);
    Serial.println(msgRX);
  }
}
