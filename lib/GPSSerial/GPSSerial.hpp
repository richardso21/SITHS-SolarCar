#pragma once

#include <Arduino.h>
#include <AltSoftSerial.h>
#include <TinyGPS++.h>
#include <TimeLib.h>

class GPSSerial : public AltSoftSerial
{
private:
    TinyGPSPlus gps;
public:
    using AltSoftSerial::AltSoftSerial;

    // encode incoming data from gps
    bool GPSencode();

    // get unix time from GPS
    time_t getUnixTime();

    // get tracked speed in MPH
    double getSpeed();

    // check if time has been updated
    bool timeUpdated();

};