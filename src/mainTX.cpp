#include "main.hpp"
TinyGPSPlus gps;
AltSoftSerial gpsSerial; //pins 8, 9, unusable 10
NeoSWSerial loraSerial(2, 3);

void sendData(String data);

void setup()
{
  // TODO: lora on AltSS, gps on SS
  Serial.begin(9600);
  // lora on hardware serial
  loraSerial.begin(9600);
  // gps on alt SS
  gpsSerial.begin(9600);
}

String msgTX;
String datetime;
double speed;
int lastSec = -1;

void loop()
{
  // read and send data once gps is available
  while (gpsSerial.available() > 0)
  {
    // encode gps serial data
    if (gps.encode(gpsSerial.read()))
    {
      if (gps.time.isValid() && gps.time.second() == lastSec)
        continue;
      lastSec = gps.time.second();
      // get speed and datetime from gps
      if (gps.speed.isValid())
        speed = gps.speed.mph();
      if (gps.date.isValid())
        datetime = String(gps.date.value()) + String(gps.time.value());
      // DDMMYYHHmmSSCC

      msgTX = String(speed) + ";" + datetime;

      Serial.println(msgTX);
      sendData(msgTX);
      // delay(2000);
    }
  }
}

// sends string data to lora radio
void sendData(String data)
{
  int dataSize = data.length();
  loraSerial.print("AT+SEND=0," + String(dataSize) + "," + data + "\r\n");
}