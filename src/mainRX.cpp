#include "main.hpp"
LiquidCrystal_I2C lcd(0x27, 20, 4);
AltSoftSerial loraSerial;

void setup()
{
  Serial.begin(9600);
  loraSerial.begin(115200);
  loraSerial.print("AT+PARAMETER=12,4,1,7\r\n");
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
