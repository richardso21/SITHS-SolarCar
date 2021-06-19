#include "main.hpp"
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(115200);
  Serial.print("AT+PARAMETER=12,4,1,7\r\n");
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
