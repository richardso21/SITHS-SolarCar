#include "ADCObject.hpp"

byte ADE7912::readByte(int ss, byte readFrom)
{
    byte res;
    digitalWrite(ss, LOW);
    transfer(readFrom | READ);
    res = transfer(DC_BYTE);
    digitalWrite(ss, HIGH);
    return res;
}

void ADE7912::readBytes(int ss, byte readFrom, int len, byte readTo[])
{
    digitalWrite(ss, LOW);
    transfer(readFrom | READ);
    for (int i = 0; i < len; i++)
        readTo[i] = transfer(DC_BYTE);
    digitalWrite(ss, HIGH);
}

void ADE7912::writeByte(int ss, byte writeTo, byte writeMsg)
{
    digitalWrite(ss, LOW);
    transfer(writeTo);
    transfer(writeMsg);
    digitalWrite(ss, HIGH);
}

void ADE7912::init(SPISettings spiSettings, int dReadyPin, int ss1, int ss2)
{
    // set pins and settings to private variables
    _dReadyPin = dReadyPin;
    _ss1 = ss1;
    _ss2 = ss2;
    _spiSettings = spiSettings;
    // _spiSettings = &SPISettings(clock, MSBFIRST, SPI_MODE3);

    // pin modes for ss
    pinMode(_ss1, OUTPUT);
    digitalWrite(_ss1, HIGH);
    pinMode(_ss2, OUTPUT);
    digitalWrite(_ss2, HIGH);
    begin();

    // begin power up procedure
    powerUp();
}

void ADE7912::powerUp()
{
    beginTransaction(_spiSettings);

    bool resetA = true;
    bool resetB = true;
    // see when ADC A is done resetting
    while (resetA)
    {
        resetA = readByte(_ss1, STATUS0) & 1;
        Serial.println(resetA);
        delay(20);
    }

    // unlock config register
    writeByte(_ss1, LOCK_REG, UNLOCK_BYTE);

    Serial.println(readByte(_ss1, CONFIG));

    // set config for ADC A to have CLKOUT functionality
    writeByte(_ss1, CONFIG, 0b00110001);

    // TODO: writeByte is not setting the byte properly
    Serial.println(readByte(_ss1, CONFIG));

    // set EMI_CTRL for ADC A
    writeByte(_ss1, EMI_CTRL, EMI_A);

    // repeat for ADC B
    while (resetB)
    {
        resetB = readByte(_ss2, STATUS0) & 1;
        Serial.println(resetB);
        delay(20);
    }
    writeByte(_ss2, LOCK_REG, UNLOCK_BYTE);
    writeByte(_ss2, CONFIG, 0b00110000);
    writeByte(_ss2, EMI_CTRL, EMI_B);

    // execute SYNC_SNAP for both ADCs
    writeByte(_ss1, SYNC_SNAP, SYNC);
    writeByte(_ss2, SYNC_SNAP, SYNC);

    // lock registers for both ADCs
    writeByte(_ss1, LOCK_REG, LOCK_BYTE);
    writeByte(_ss2, LOCK_REG, LOCK_BYTE);

    // Serial.println("ADCs Configured!");

    endTransaction();
}

void ADE7912::burstReadData(double ADCData[])
{
    beginTransaction(_spiSettings);

    byte dataA[6];
    byte dataB[6];
    long mBatt, mArr, mMot;
    // double res[3];

    // read IWV and V1 from both ADCs
    readBytes(_ss1, IWV, 6, dataA);
    readBytes(_ss2, IWV, 6, dataB);

    // Serial.print(dataA[0]);
    // Serial.print(" ");
    // Serial.print(dataA[1]);
    // Serial.print(" ");
    // Serial.print(dataA[2]);
    // Serial.print(" ");
    // Serial.print(dataA[3]);
    // Serial.print(" ");
    // Serial.print(dataA[4]);
    // Serial.print(" ");
    // Serial.print(dataA[5]);
    // Serial.print(" ");

    endTransaction();

    // translate arrays of bytes to 24-bit signed longs
    mBatt = translateDataBytes(dataA, 0, 3);
    mArr = translateDataBytes(dataA, 3, 6);
    // long mMisc = translateDataBytes(dataB, 0, 3);
    mMot = translateDataBytes(dataB, 3, 6);
    // TODO: screwed up byte order (i.e. mBatt <-> mArr)

    ADCData[0] = map(mBatt, LRANGE, HRANGE, -V_MAX, V_MAX);
    ADCData[1] = map(mArr, LRANGE, HRANGE, -C_MAX, C_MAX) / 1000 / SH_RES;
    ADCData[2] = map(mMot, LRANGE, HRANGE, -C_MAX, C_MAX) / 1000 / SH_RES;
}

long ADE7912::translateDataBytes(byte byteArr[], int begin, int end)
{
    // check if signed byte is a negative
    bool neg = byteArr[begin] & (1 << 7);

    long val = 0;
    // shift one byte left for next byte
    for (int i = begin; i < end; i++)
        val = (val << 8) | byteArr[i];
    // use two's complement to convert unsigned to signed
    if (neg)
        val = -(~(val - 1) & 0xFFFFFF);
    return val;
}
