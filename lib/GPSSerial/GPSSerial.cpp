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

bool GPSSerial::secondUpdated()
{
    time_t rn = gps.time.value();
    if (rn > _secondUpdate)
    {
        _secondUpdate = rn;
        return true;
    }
    return false;
}

int GPSSerial::getSpeed()
{
    if (gps.speed.isValid())
        return (int)round(gps.speed.mph());
    else
        return 0;
}

bool GPSSerial::gpsLocked()
{
    return (gps.satellites.isValid() && (gps.satellites.value() > 0));
}