#include "main.hpp"
LiquidCrystal_I2C lcd(0x27, 20, 4);
NeoSWSerial loraSerial(8, 9);

String parseData();

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
    msgRX = parseData();
    lcd.clear();
    lcd.println(msgRX);
    Serial.println(msgRX);
  }
}

String parseData()
{
  String msg = loraSerial.readString();

  String parsed[5];
  int n = 0;
  int length = msg.length();
  String tmp;
  for (int i = 0; i < length; i++)
  {
    char c = msg[i];
    if (c == ',')
    {
      parsed[n] = tmp;
      tmp = "";
      n++;
    }
    else
    {
      tmp += String(c);
    }
  }
  return parsed[2];
}