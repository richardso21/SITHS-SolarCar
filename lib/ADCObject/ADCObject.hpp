#pragma once

#include <Arduino.h>
#include <SPI.h>

// misc bytes
#define DC_BYTE 0x00
#define LOCK_BYTE 0xCA
#define UNLOCK_BYTE 0x9C
#define EMI_A 0x55
#define EMI_B 0xAA
#define SYNC 0x01
#define SNAP 0x02
// misc variables
#define MAX_TRY 100
#define READ 0b100
#define LRANGE -8388608
#define HRANGE 8388607
#define C_MAX 49.27     // max mV from IM/IP
#define V_MAX 788       // max mV from V1
#define SH_RES 0.00075
// registers
#define STATUS0 (0x9 << 3)
#define EMI_CTRL (0xE << 3)
#define CONFIG (0x8 << 3)
#define LOCK_REG (0xA << 3)
#define SYNC_SNAP (0xB << 3)
#define IWV (0x0 << 3)
#define V1WV (0x1 << 3)
#define V2WV (0x2 << 3)
#define ADC_CRC (0x4 << 3)

// --- IMPORTANT ---
// This class and all its methods assumes a setup of two
// ADE7912's clocked with an external 4.096 MHz crystal
class ADE7912 : public SPIClass
{
private:
    int _dReadyPin, _ss1, _ss2;
    SPISettings _spiSettings;

    // read and return one byte from a register
    byte readByte(int ss, byte readFrom);

    // read bytes from a register to a byte array
    void readBytes(int ss, byte readFrom, int len, byte readTo[]);

    // write a byte onto a register
    void writeByte(int ss, byte writeTo, byte writeMsg);

public:
    // start SPI bus for ADC with specificed clock frequency
    void init(SPISettings spiSettings, int dReadyPin, int ss1, int ss2);

    // run power-up procedure
    void powerUp();

    // get values of data
    void burstReadData(double ADCData[]);

    // translate an array of bytes into long
    long translateDataBytes(byte byteArr[], int begin, int end);
};
