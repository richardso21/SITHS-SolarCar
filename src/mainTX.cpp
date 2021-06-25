#include "main.hpp"
TinyGPSPlus gps;
AltSoftSerial gpsSerial; //pins 8, 9, unusable 10
NeoSWSerial loraSerial(2, 3);

String getDataFromGPS();
void sendData(String data);

void setup()
{
  Serial.begin(9600);
  // initiate lora and GPS serial coms
  loraSerial.begin(9600);
  gpsSerial.begin(9600);
}

String msgBatch;
int n = 0;
String DT;
double speed;
int prevSec = -1;

void loop()
{
  // read and send data once gps is available
  while (gpsSerial.available())
  {
    // encode gps serial data
    if (gps.encode(gpsSerial.read()))
    {
      // ignore data if on same second
      if (gps.time.isValid() && gps.time.second() == prevSec)
        continue;
      prevSec = gps.time.second();

      // get data from GPS
      String msg = getDataFromGPS();
      Serial.println(msg);
      msgBatch += msg + ":";
      n++;
    }
    // send 4-second batch of data
    if (n >= 4)
    {
      sendData(msgBatch);
      msgBatch = "";
      n = 0;
    }
  }
}

// get speed and datetime from gps
String getDataFromGPS()
{
  if (!(gps.speed.isValid() && gps.date.isValid()))
    return "ERR!";

  double speed = gps.speed.mph();
  setTime(gps.time.hour(),
          gps.time.minute(),
          gps.time.second(),
          gps.date.day(),
          gps.date.month(),
          gps.date.year());
  time_t timedelta = now();
  return String(speed) + ";" + String(timedelta);
}

// sends string data to lora radio
void sendData(String data)
{
  digitalWrite(13, HIGH);
  int dataSize = data.length();
  loraSerial.print("AT+SEND=0," + String(dataSize) + "," + data + "\r\n");
  digitalWrite(13, LOW);
}