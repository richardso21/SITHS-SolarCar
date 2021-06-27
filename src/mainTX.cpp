#include "main.hpp"

GPSSerial gpsSerial;
LoraSerial lora(2, 3);
LCD lcd(0x27, 20, 4);

void setup()
{
  // initiate serial coms
  Serial.begin(9600);
  lora.begin(38400);
  gpsSerial.begin(9600);
  // initialize lcd
  lcd.start(true);
  lcd.dprint("Locking Position...");
}

void loop()
{
  // read and send data once gps is available
  while (gpsSerial.available())
  {
    // encode gps data from serial
    if (gpsSerial.GPSencode())
    {
      lcd.dclear();
      // if gps is locking position or collecting on same second, pass
      if (!gpsSerial.gpsLocked() || !gpsSerial.secondUpdated())
        continue;

      // get data from GPS
      time_t currentTime = gpsSerial.getUnixTime();
      int speed = gpsSerial.getSpeed();

      // print to lcd
      lcd.displayData(currentTime, speed, 0, 0, 0, 0);
      // lcd.displayData(currentTime, 30, 100, 12.5, 12.5, 13.5);

      // transmit data over 2-second intervals
      if (gpsSerial.intervalUpdated(2))
        lora.queueData(currentTime, speed, 0, 0, 0, 0, 0);
      // lora.queueData(currentTime, 30, 100, 12.5, 12.5, 13.5, 105);
    }
  }
}
