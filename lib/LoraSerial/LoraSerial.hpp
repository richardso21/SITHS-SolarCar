#pragma once

#include <Arduino.h>
#include <NeoSWSerial.h>
#include <TimeLib.h>

#define BATCH_SIZE 3

class LoraSerial : public NeoSWSerial
{
private:
    String dataBuffer = "";
    int _c = 0;
public:
    using NeoSWSerial::NeoSWSerial;

    // transmitts input string via lora
    void sendData(String msg);
    void sendData(
        time_t unixTime,
        int speed,
        int vBatt,
        double vAux,
        double aMot,
        double aShunt,
        int temp);

    // queue's data into buffer
    void queueData(String msg);
    void queueData(
        time_t unixTime,
        int speed,
        int vBatt,
        double vAux,
        double aMot,
        double aShunt,
        int temp);

    // reads received data and strips unwanted formatting
    String parseData();
};