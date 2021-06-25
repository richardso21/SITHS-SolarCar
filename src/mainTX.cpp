#include "main.hpp"
GPSSerial gps;
LoraSerial lora(2, 3);

void setup()
{
  Serial.begin(9600);
  // initiate lora and GPS serial coms
  lora.begin(9600);
  gps.begin(9600);
}

String msgBatch;
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
      // get data from GPS
      time_t currentTime = gps.getUnixTime();
      double speed = gps.getSpeed();
      // ignore data if on same second
      if (currentTime == lastUpdate)
        continue;
      lastUpdate = currentTime;
      // print data to lora radio
      String msg = String(speed) + ";" + String(currentTime);
      Serial.println(msg);
      msgBatch += msg + ":";
      n++;
    }
    // send 4-second batch of data
    if (n >= 4)
    {
      lora.sendData(msgBatch);
      msgBatch = "";
      n = 0;
    }
  }
}
