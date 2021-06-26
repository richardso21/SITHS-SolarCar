#include "main.hpp"

GPSSerial gps;
LoraSerial lora(2, 3);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  // initiate serial coms
  Serial.begin(9600);
  lora.begin(9600);
  gps.begin(9600);
  // initialize lcd
  lcd.init();
  lcd.backlight();
}

String msgBatch[BATCH_SIZE];
int n = 0;
time_t lastUpdate = -1;

void loop()
{
  // read and send data once gps is available
  while (gps.available())
  {
    // encode gps data from serial
    if (gps.GPSencode())
    {
      // if gps is still locking position, pass
      if (!gps.gpsLocked())
        continue;

      // get data from GPS
      time_t currentTime = gps.getUnixTime();
      double speed = gps.getSpeed();

      // ignore data if on same second
      if (currentTime == lastUpdate)
        continue;
      lastUpdate = currentTime;

      // print to lcd
      lcd.clear();
      lcd.print("SP: " + String(speed));
      lcd.setCursor(0, 2);
      lcd.print("DT: " + String(currentTime));

      // store snapshot to msgBatch
      String msg = String(currentTime) + ";" + String(speed);
      Serial.println(msg);
      msgBatch[n] = msg;
      n++;
    }
    // send data after reaching batch size
    if (n >= BATCH_SIZE)
    {
      lcd.setCursor(19, 3);
      lcd.print("^");
      lora.sendData(msgBatch);
      n = 0;
    }
  }
}
