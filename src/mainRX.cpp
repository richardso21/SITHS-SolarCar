#include "main.hpp"
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
}

String msgRX;

void loop()
{
  if (Serial.available())
  {
    msgRX = Serial.readString();
    lcd.clear();
    lcd.println(msgRX);
  }
}
