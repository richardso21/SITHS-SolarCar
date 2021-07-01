#include "main.hpp"

// #define INTERVAL_SEC 0
#define V_REF 1.1
#define R1 20
#define R2 1
#define VDIV_RATIO (R1 + R2) / R2

GPSSerial gpsSerial;
LoraSerial lora(2, 3);
LCD lcd(0x27, 20, 4);

int mainFunc(bool gpsEnabled);

void setup()
{
  // initiate serial coms
  Serial.begin(9600);
  lora.begin(9600);
  gpsSerial.begin(9600);
  // initialize lcd
  lcd.start(true);
  // set internal reference voltage to 1.1
  analogReference(INTERNAL);
}

void loop()
{
  while (gpsSerial.available())
    if (gpsSerial.GPSencode())
      mainFunc(true);
  // if gps is missing / not encoding stuff
  if (millis() > 5000 && gpsSerial.gps.charsProcessed() < 10)
    while (true)
      mainFunc(false);
}

unsigned long _lastLoraTX = 0;

int mainFunc(bool gpsEnabled)
{
  // measure aux voltage (arduino's own voltage)
  double vAux = (double)analogRead(A0) * (1.1 / 1023.0) * 21.0;

  // get current mp time, check if time to send
  // unsigned long rn = millis();
  // bool send = rn >  _lastLoraTX + (INTERVAL_SEC * 1000);

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
    // indicate no GPS, nullify time and speed
    lcd.indCondPrint("NO GPS!", 0, true);
    currentTime = 0;
    speed = 0;
  }

  // print to lcd
  lcd.displayData(currentTime, speed, 0, vAux, 0, 0);

  // send data via lora
  lora.sendData(currentTime, speed, 0, vAux, 0, 0, 0);
  // lcd.indCondPrint("SENT", 15, send);
  // if (send)
  // {
  //   lora.sendData(currentTime, speed, 0, vAux, 0, 0, 0);
  //   _lastLoraTX = rn;
  // }

  return 0;
}