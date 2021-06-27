#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>

class LCD : public LiquidCrystal_I2C
{
private:
public:
    using LiquidCrystal_I2C::LiquidCrystal_I2C;

    void start(bool bl);

    void formatSetup();

    void dprint(String msg);

    void dclear();

    void displayData(
        time_t unixTime,
        int speed,
        int vBatt,
        double vAux,
        double aMot,
        double aShunt);
    void displayData(bool dataReceived);
};