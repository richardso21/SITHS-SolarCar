#include "main.hpp"
LiquidCrystal_I2C lcd(0x27, 20, 4);
LoraSerial lora(8, 9);

void setup()
{
  Serial.begin(9600);
  lora.begin(9600);
  lcd.init();
  lcd.backlight();
}

String msgRX;

void loop()
{
  if (lora.available())
  {
    msgRX = lora.parseData();
    lcd.clear();
    lcd.println(msgRX);
    Serial.println(msgRX);
  }
}
