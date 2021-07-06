#include "ADCObject.hpp"

void ADCObject::begin(int dReadyPin, int ss1, int ss2)
{
    _dReadyPin = dReadyPin;
    _ss1 = ss1;
    _ss2 = ss2;

    SPISettings spiSettings(4096000, MSBFIRST, SPI_MODE3);
    SPIClass::begin();
    SPIClass::beginTransaction(spiSettings);
}

bool ADCObject::powerUpCheck()
{
    
}
