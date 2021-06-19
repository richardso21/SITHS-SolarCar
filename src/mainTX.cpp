#include "main.hpp"
TinyGPSPlus gps;
AltSoftSerial gpsSerial; // pins 8 (RX), 9 (TX)

void setup()
{
  // lora on hardware serial
  Serial.begin(115200);
  Serial.print("AT+PARAMETER=12,4,1,7\r\n");
  // gps on alt SS
  gpsSerial.begin(9600);
}

String msgTX;
String datetime;
double speed;
int dataSize;

void loop()
{
  // read and send data once gps is available
  while (gpsSerial.available())
  {
    // encode gps serial data
    gps.encode(gpsSerial.read());

    // get speed and datetime from gps
    if (gps.speed.isValid())
      speed = gps.speed.mph();
    if (gps.time.isValid() && gps.date.isValid())
      datetime = String(gps.date.value()) + String(gps.time.value());

    msgTX = String(speed) + ";" + datetime;
    // 2 (speed) + 14 (datetime) + 1 (;) = 27 bytes

    dataSize = msgTX.length();
    Serial.print("AT+SEND=0," + String(dataSize) + "," + msgTX + "\r\n");
    delay(1000);
  }
}
