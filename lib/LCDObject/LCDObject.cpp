#include "LCDObject.hpp"

void LCDObject::start(bool bl)
{
    LCDObject::init();
    if (bl)
        LCDObject::backlight();
    LCDObject::formatSetup();
}

void LCDObject::formatSetup()
{
    LCDObject::clear();
    // Line 1
    LCDObject::setCursor(15, 0);
    LCDObject::print("MPH");
    // Line 2
    LCDObject::setCursor(0, 2);
    LCDObject::print("BAT:");
    LCDObject::setCursor(10, 2);
    LCDObject::print("AUX:");
    // Line 3
    LCDObject::setCursor(0, 3);
    LCDObject::print("MOT:");
    LCDObject::setCursor(10, 3);
    LCDObject::print("ARR:");
}

void LCDObject::indPrint(String msg, int col)
{
    LCDObject::setCursor(col, 1);
    LCDObject::print(msg);
}

void LCDObject::indCondPrint(String msg, int col, bool condition)
{
    unsigned long rn = millis();
    if (condition)
    {
        if (rn > _prevIndPrint)
            _prevIndPrint = rn;
        LCDObject::indPrint(msg, col);
    }
    else if (rn >= (_prevIndPrint + 500))
        LCDObject::indClear(col, msg.length());
}

void LCDObject::indClear()
{
    LCDObject::setCursor(0, 1);
    LCDObject::print("                    ");
}

void LCDObject::indClear(int col, int length)
{
    LCDObject::setCursor(col, 1);
    for (int i = 0; i < length; i++)
        LCDObject::print(" ");
}

String formatInt(int i, int padding)
{

    String res;
    int places;
    if (i == 0)
        places = 1;
    else
        places = log10(i) + 1;
    if (places >= padding)
        return String(i);
    for (int i = 0; i < (padding - places); i++)
        res += "0";
    res += String(i);
    return res;
}

void LCDObject::displayData(
    time_t unixTime,
    int speed,
    int vBatt,
    double vAux,
    double aMot,
    double aShunt)
{
    // format time (HH:MM:SS)
    String TF[3] = {
        formatInt(hour(unixTime), 2),
        formatInt(minute(unixTime), 2),
        formatInt(second(unixTime), 2)};

    // Line 1
    LCDObject::setCursor(0, 0);
    LCDObject::print(TF[0] + ":" + TF[1] + ":" + TF[2]);
    LCDObject::setCursor(12, 0);
    LCDObject::print(formatInt(speed, 2));
    // Line 2
    LCDObject::setCursor(4, 2);
    LCDObject::print(formatInt(vBatt, 3));
    LCDObject::print("V");
    LCDObject::setCursor(14, 2);
    LCDObject::print(vAux, 2);
    LCDObject::print("V");
    // Line 3
    LCDObject::setCursor(4, 3);
    LCDObject::print(aMot, 1);
    LCDObject::print("A");
    LCDObject::setCursor(14, 3);
    LCDObject::print(aShunt, 1);
    LCDObject::print("A");
}
