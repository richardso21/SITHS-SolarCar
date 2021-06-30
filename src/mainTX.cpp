#include "main.hpp"

GPSSerial gpsSerial;
LoraSerial lora(2, 3);
LCD lcd(0x27, 20, 4);

bool firstSend = true;

int mainFunc(bool gpsEnabled);

void setup()
{
  // initiate serial coms
  Serial.begin(9600);
  lora.begin(9600);
  gpsSerial.begin(9600);
  // initialize lcd
  lcd.start(true);
  lcd.indPrint("/", 18);
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

int mainFunc(bool gpsEnabled)
{
  bool received = lora.receivedConfirm();
  // indicate if received
  lcd.indCondPrint("R", 17, received);
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
  lcd.displayData(currentTime, speed, 0, 0, 0, 0);

  // send to RX during first send or after received confirmation
  if (received || firstSend)
  {
    lora.sendData(currentTime, speed, 0, 0, 0, 0, 0);
    // 1010101010;00;000;00.0;00.0;00.0;00 (36 characters)
    firstSend = false;
  }

  lcd.indCondPrint("S", 19, received || firstSend);

  return 0;
}