#pragma once

#include <Arduino.h>
#include <SPI.h>

// misc bytes
#define DC_BYTE 0x00
#define LOCK_BYTE 0xCA
#define UNLOCK_BYTE 0x9C
// misc variables
#define MAX_TRY 100
#define READ 0b100
#define WRITE 0b000
// registers
#define STATUS0 (0x9 << 3)
#define EMI_CTRL (0xE << 3)
#define IVW_READ (0x0 << 3)
#define V1WV_READ (0x1 << 3)
#define V2WV_READ (0x2 << 3)
#define ADC_CRC_READ (0x2 << 3)

// --- IMPORTANT ---
// This class and all its methods assumes a setup of two
// ADE7912's clocked with an external 4.096 MHz crystal
class ADCObject : public SPIClass
{
private:
    int _dReadyPin, _ss1, _ss2;

    void readBytes(int ss, byte readFrom, int len, byte *data);

    byte readByte(int ss, byte readFrom);

    void writeByte(int ss, byte writeTo, byte writeMsg);

public:
    // start ADC SPI with optimal settings
    void begin(int dReadyPin, int ss1, int ss2);

    // run power-up procedure and return if complete
    bool powerUpCheck();
};