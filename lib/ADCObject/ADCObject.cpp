#include "ADCObject.hpp"

void ADCObject::readBytes(int ss, byte readFrom, int len, byte *data)
{
    digitalWrite(ss, LOW);
    SPI.transfer(readFrom | READ);
    // for (int i = (len - 1); i <= 0; i--)
    for (int i = 0; i < len; i++)
        data[i] = SPI.transfer(DC_BYTE);
    digitalWrite(ss, HIGH);
}

byte ADCObject::readByte(int ss, byte readFrom)
{
    byte res;
    digitalWrite(ss, LOW);
    res = SPI.transfer(readFrom | READ);
    digitalWrite(ss, HIGH);
    return res;
}

void ADCObject::writeByte(int ss, byte writeTo, byte writeMsg)
{
    digitalWrite(ss, LOW);
    SPI.transfer(writeTo);
    SPI.transfer(writeMsg);
    digitalWrite(ss, HIGH);
}

void ADCObject::begin(int dReadyPin, int ss1, int ss2)
{
    // set pins to private variables
    _dReadyPin = dReadyPin;
    _ss1 = ss1;
    _ss2 = ss2;

    // pin modes for ss and dready
    pinMode(_ss1, OUTPUT);
    digitalWrite(_ss1, HIGH);
    pinMode(_ss2, OUTPUT);
    digitalWrite(_ss2, HIGH);
    // pinMode(_dReadyPin, INPUT_PULLUP);

    SPISettings spiSettings(4096000, MSBFIRST, SPI_MODE3);
    SPIClass::begin();
    SPIClass::beginTransaction(spiSettings);
}

bool ADCObject::powerUpCheck()
{
    bool reset = true;
    // see when ADC A is done resetting
    while (reset)
        reset = readByte(_ss1, STATUS0) & 1;
    // 
    
}
