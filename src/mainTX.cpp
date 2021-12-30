#include "main.hpp"

#define DREADY 5
#define ADCA 7
#define ADCB 6
#define INTERVAL_SEC 15
#define V_REF 1.1
#define R1 20
#define R2 1
#define VDIV_RATIO (R1 + R2) / R2

// GPSSerial gpsSerial;
// LoraSerial lora(2, 3);
// LCDObject lcd(0x27, 20, 4);
ADE7912 adc;

unsigned long _lastSend = 0;
unsigned long _lastGPSCheck = 0;

int mainFunc(bool gpsEnabled);

void setup()
{
    // initiate serial coms
    Serial.begin(9600);
    // >>> COMMENT BELOW IF TESTING ADC >>>
    // lora.begin(9600);
    // gpsSerial.begin(9600);
    // // initialize lcd
    // lcd.start(true);
    // // set reference voltage to internal 1.1v
    // analogReference(INTERNAL);
    // analogRead(A0);
    // <<< COMMENT ABOVE IF TESTING ADC <<<

    SPISettings spiSettings(250000, MSBFIRST, SPI_MODE3);
    adc.init(spiSettings, DREADY, ADCA, ADCB);
}

void loop()
{
    /* while (gpsSerial.available())
        if (gpsSerial.GPSencode())
            mainFunc(true);
    // if gps is missing / not encoding stuff
    if (millis() > 5000 && gpsSerial.gps.charsProcessed() < 10)
    {
        _lastGPSCheck = millis();
        // check again after 10 seconds
        while ((millis() - _lastGPSCheck) < 10000)
            mainFunc(false);
    } */
    // Serial.println("testing");
    mainFunc(false);
    delay(1000);
    
}

int mainFunc(bool gpsEnabled)
{
    double ADCData[3];
    adc.burstReadData(ADCData);
    Serial.println(ADCData[0]);
    Serial.println(ADCData[1]);
    Serial.println(ADCData[2]);
    // measure aux voltage (arduino's own voltage)
  /*   double vAux = (double)analogRead(A0) * (V_REF / 1023.0) * VDIV_RATIO;

    time_t currentTime;
    int speed;

    if (gpsEnabled)
    {
        // if gps is locking, pass and indicate
        bool locking = !gpsSerial.gpsLocked();
        lcd.indCondPrint("LOCKING", 0, locking);
        if (locking)
            return 1;

        // if gps collecting on same second, pass
        // if (!gpsSerial.secondUpdated())
        //     return 1;

        // get data from GPS
        currentTime = gpsSerial.getUnixTime();
        speed = gpsSerial.getSpeed();
    }
    else
    {
        // indicate no GPS, nullify time and speed
        lcd.indCondPrint("NO GPS!", 0, true);
        currentTime = 0;
        speed = 0;
        // delay for 1 sec (substitutes `secondsUpdated`)
        // delay(1000);
    }

    // print to lcd
    lcd.displayData(currentTime, speed, ADCData[0], vAux, ADCData[1], ADCData[2]);

    // send data via lora
    if (millis() > (_lastSend + (INTERVAL_SEC * 1000)))
    {
        _lastSend = millis();
        lora.sendData(speed, ADCData[0], vAux, ADCData[1], ADCData[2]);
    }
 */
    return 0; 
}