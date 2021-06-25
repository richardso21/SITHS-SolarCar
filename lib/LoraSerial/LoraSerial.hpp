#pragma once

#include <Arduino.h>
#include <NeoSWSerial.h>

class LoraSerial : public NeoSWSerial
{
public:
    using NeoSWSerial::NeoSWSerial;

    // transmitts input string via lora
    void sendData(String msg);
    // void sendData(String msgs[]);

    // reads received data and strips unwanted formatting
    String parseData();
};