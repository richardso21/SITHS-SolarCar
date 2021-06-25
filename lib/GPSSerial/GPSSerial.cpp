#include "GPSSerial.hpp"

bool GPSSerial::GPSencode()
{
    return gps.encode(GPSSerial::read());
}

time_t GPSSerial::getUnixTime()
{
    if (gps.time.isValid() && gps.date.isValid())
    {
        setTime(gps.time.hour(),
                gps.time.minute(),
                gps.time.second(),
                gps.date.day(),
                gps.date.month(),
                gps.date.year());
        return now();
    }
    else
        return 0;
}

double GPSSerial::getSpeed()
{
    if (gps.speed.isValid())
        return gps.speed.mph();
    else
        return 0;
}

bool GPSSerial::timeUpdated()
{
    return gps.time.isUpdated();
}