#include "ADCObject.hpp"

byte ADCObject::readByte(int ss, byte readFrom)
{
    byte res;
    digitalWrite(ss, LOW);
    res = transfer(readFrom | READ);
    digitalWrite(ss, HIGH);
    return res;
}

void ADCObject::readBytes(int ss, byte readFrom, int len, byte *data)
{
    digitalWrite(ss, LOW);
    transfer(readFrom | READ);
    // for (int i = (len - 1); i <= 0; i--)
    for (int i = 0; i < len; i++)
        data[i] = transfer(DC_BYTE);
    digitalWrite(ss, HIGH);
}

void ADCObject::writeByte(int ss, byte writeTo, byte writeMsg)
{
    digitalWrite(ss, LOW);
    transfer(writeTo);
    transfer(writeMsg);
    digitalWrite(ss, HIGH);
}

void ADCObject::init(int dReadyPin, int ss1, int ss2)
{
    // set pins to private variables
    _dReadyPin = dReadyPin;
    _ss1 = ss1;
    _ss2 = ss2;
    // _spiSettings = &SPISettings(4096000, MSBFIRST, SPI_MODE3);
    _spiSettings = &SPISettings(2500000, MSBFIRST, SPI_MODE3);

    // pin modes for ss
    pinMode(_ss1, OUTPUT);
    digitalWrite(_ss1, HIGH);
    pinMode(_ss2, OUTPUT);
    digitalWrite(_ss2, HIGH);
    begin();
}

void ADCObject::powerUp()
{
    beginTransaction(*_spiSettings);

    bool resetA = true;
    bool resetB = true;
    // see when ADC A is done resetting
    while (resetA)
        resetA = readByte(_ss1, STATUS0) & 1;

    // unlock config register
    writeByte(_ss1, LOCK_REG, UNLOCK_BYTE);

    // set config for ADC A to have CLKOUT functionality
    writeByte(_ss1, CONFIG, 0b00110001);

    // set EMI_CTRL for ADC A
    writeByte(_ss1, EMI_CTRL, EMI_A);

    // repeat for ADC B
    while (resetB)
        resetB = readByte(_ss2, STATUS0) & 1;
    writeByte(_ss2, LOCK_REG, UNLOCK_BYTE);
    writeByte(_ss2, CONFIG, 0b00110000);
    writeByte(_ss2, EMI_CTRL, EMI_B);

    // execute SYNC_SNAP for both ADCs
    writeByte(_ss1, SYNC_SNAP, SYNC);
    writeByte(_ss2, SYNC_SNAP, SYNC);

    // lock registers for both ADCs
    writeByte(_ss1, LOCK_REG, LOCK_BYTE);
    writeByte(_ss2, LOCK_REG, LOCK_BYTE);

    endTransaction();
}

void ADCObject::burstReadData(double *ADCData)
{
    beginTransaction(*_spiSettings);
    byte dataA[6];
    byte dataB[6];
    double resA[2];
    double resB[2];

    // read IWV and V1 from both ADCs
    readBytes(_ss1, IWV, 6, dataA);
    readBytes(_ss2, IWV, 6, dataB);

    translateDataBytes(dataA, resA, 249000, 1);
    translateDataBytes(dataB, resB, 100, 1);

    endTransaction();
}

void ADCObject::translateDataBytes(byte *data, double *res, uint64_t r1, uint64_t r2)
{
    int64_t iwv_res = 0;
    int64_t v1_res = 0;

    for (int i = 0; i < 3; i++)
        iwv_res = (iwv_res << 8) | data[i];
    for (int i = 3; i < 6; i++)
        v1_res = (v1_res << 8) | data[i];
    
    res[0] = iwv_res;

}
