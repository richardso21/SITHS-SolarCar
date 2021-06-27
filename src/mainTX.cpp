#include "main.hpp"

GPSSerial gpsSerial;
LoraSerial lora(2, 3);
LCD lcd(0x27, 20, 4);

int mainFunc(bool gpsEnabled);

void setup()
{
  // initiate serial coms
  Serial.begin(9600);
  lora.begin(38400);
  gpsSerial.begin(9600);
  // initialize lcd
  lcd.start(true);
}

void loop()
{
  while (gpsSerial.available())
    if (gpsSerial.GPSencode())
      mainFunc(true);
  if (millis() > 5000 && gpsSerial.gps.charsProcessed() < 10)
    while(true)
      mainFunc(false);
}

int mainFunc(bool gpsEnabled)
{
  time_t currentTime;
  int speed;

  if (gpsEnabled)
  {
    // if gps is locking, pass and indicate
    bool locking = !gpsSerial.gpsLocked();
    lcd.indCondPrint("LOCKING", 0, locking);
    if (locking)
      return 1;

    // if gps collecting on same second, pass
    if (!gpsSerial.secondUpdated())
      return 1;

    // get data from GPS
    currentTime = gpsSerial.getUnixTime();
    speed = gpsSerial.getSpeed();
  }
  else
  {
    lcd.indCondPrint("NO GPS", 0, true);
    currentTime = 0;
    speed = 0;
  }

  // print to lcd
  lcd.displayData(currentTime, speed, 0, 0, 0, 0);

  // transmit data over 2-second intervals
  if (gpsSerial.intervalUpdated(2))
    lora.queueData(currentTime, speed, 0, 0, 0, 0, 0);

  // indicate if data batch is sent
  lcd.indCondPrint("S", 18, lora.hasSent());

  return 0;
}