#pragma once

#include <Arduino.h>
#include <SPI.h>

// misc bytes
#define DC_BYTE 0x00
#define LOCK_BYTE 0xCA
#define UNLOCK_BYTE 0x9C

#define READ 0b100
#define WRTE 0b000

// read cmds
#define STATUS0_READ (0x9 << 3 | READ)

// This class and all its methods assumes a setup of two
// ADE7912's clocked with an external 4.096 MHz crystal
class ADCObject : public SPIClass
{
private:
    int _dReadyPin, _ss1, _ss2;

public:
    // start ADC SPI with optimal settings
    void begin(int dReadyPin, int ss1, int ss2);

    // run power-up procedure and return if complete
    bool powerUpCheck();


};