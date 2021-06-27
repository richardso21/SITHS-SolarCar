#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>

class LCD : public LiquidCrystal_I2C
{
private:
    unsigned long _prevIndPrint = 0;
public:
    using LiquidCrystal_I2C::LiquidCrystal_I2C;

    // init and backlight lcd
    void start(bool bl);

    // format static characters in display
    void formatSetup();

    // print on reserved indicator row 
    void indPrint(String msg, int col);

    // print on indicator, given a truthy condition
    void indCondPrint(String msg, int col, bool condition);

    // clear whole indicator row
    void indClear();

    // clear denoted portion of indicator row
    void indClear(int col, int length);

    // display variable data onto display
    void displayData(
        time_t unixTime,
        int speed,
        int vBatt,
        double vAux,
        double aMot,
        double aShunt);
};