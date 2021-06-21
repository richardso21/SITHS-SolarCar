#include "main.hpp"
TinyGPSPlus gps;
AltSoftSerial loraSerial; //pins 8, 9, unusable 10
NeoSWSerial gpsSerial(2, 3);

void sendData(String data);

void setup()
{
  // TODO: lora on AltSS, gps on SS

  // lora on hardware serial
  loraSerial.begin(115200);
  loraSerial.print("AT+PARAMETER=12,4,1,7\r\n");
  // gps on alt SS
  gpsSerial.begin(9600);
}

String msgTX;
String datetime;
double speed;

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
      // DDMMYYHHmmSSCC

    msgTX = String(speed) + ";" + datetime;
    // 2 (speed) + 14 (datetime) + 1 (;) = 27 bytes

    sendData(msgTX);
    delay(1000);
  }
}

// sends string data to lora radio
void sendData(String data)
{
  int dataSize = data.length();
  loraSerial.print("AT+SEND=0," + String(dataSize) + "," + data + "\r\n");
}