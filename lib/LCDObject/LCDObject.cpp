#include "LCDObject.hpp"

void LCD::start(bool bl)
{
    LCD::init();
    if (bl)
        LCD::backlight();
    LCD::formatSetup();
}

void LCD::formatSetup()
{
    LCD::clear();
    // Line 1
    LCD::setCursor(15, 0);
    LCD::print("MPH");
    // Line 2
    LCD::setCursor(0, 2);
    LCD::print("BAT:");
    LCD::setCursor(10, 2);
    LCD::print("AUX:");
    // Line 3
    LCD::setCursor(0, 3);
    LCD::print("MOT:");
    LCD::setCursor(10, 3);
    LCD::print("ARR:");
}

void LCD::indPrint(String msg, int col)
{
    LCD::setCursor(col, 1);
    LCD::print(msg);
}

void LCD::indCondPrint(String msg, int col, bool condition)
{
    unsigned long rn = millis();
    if (condition)
    {
        if (rn > _prevIndPrint)
            _prevIndPrint = rn;
        LCD::indPrint(msg, col);
    }
    else if (rn >= (_prevIndPrint + 500))
        LCD::indClear(col, msg.length());
}

void LCD::indClear()
{
    LCD::setCursor(0, 1);
    LCD::print("                    ");
}

void LCD::indClear(int col, int length)
{
    LCD::setCursor(col, 1);
    for (int i = 0; i < length; i++)
        LCD::print(" ");
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

void LCD::displayData(
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
    LCD::setCursor(0, 0);
    LCD::print(TF[0] + ":" + TF[1] + ":" + TF[2]);
    LCD::setCursor(12, 0);
    LCD::print(formatInt(speed, 2));
    // Line 2
    LCD::setCursor(4, 2);
    LCD::print(formatInt(vBatt, 3));
    LCD::print("V");
    LCD::setCursor(14, 2);
    LCD::print(vAux, 2);
    LCD::print("V");
    // Line 3
    LCD::setCursor(4, 3);
    LCD::print(aMot, 1);
    LCD::print("A");
    LCD::setCursor(14, 3);
    LCD::print(aShunt, 1);
    LCD::print("A");
}
