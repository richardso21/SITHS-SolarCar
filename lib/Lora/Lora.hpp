#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

class Lora
{
private:
    SoftwareSerial *loraSerial;
public:
    // accepts a pointer to the created SoftwareSerial object
    Lora(SoftwareSerial *ss);

    // begins and sets up lora module
    void begin(long baud);

    // method to send data using lora
    void send(String message);

    // reimplementing SS functions
    bool available();
    char read();
    String readString();
};

