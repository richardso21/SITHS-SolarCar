#include "LoraSerial.hpp"

void LoraSerial::sendData(String msg)
{
    int size = msg.length();
    String res = "AT+SEND=0," + String(size) + "," + msg + "\r\n";
    LoraSerial::print(res);
    _timeSent = millis();
}

void LoraSerial::queueData(String msg)
{
    dataBuffer += (msg + "|");
    _c++;
    if (_c == BATCH_SIZE)
    {
        LoraSerial::sendData(dataBuffer);
        _c = 0;
        dataBuffer = "";
    }
}

void LoraSerial::sendData(
    time_t unixTime,
    int speed,
    int vBatt,
    double vAux,
    double aMotor,
    double aShunt,
    int temp)
{
    String msg =
        String(unixTime) + ";" +
        String(speed) + ";" +
        String(vBatt) + ";" +
        String(vAux) + ";" +
        String(aMotor) + ";" +
        String(aShunt) + ";" +
        String(temp);

    LoraSerial::sendData(msg);
}

void LoraSerial::queueData(
    time_t unixTime,
    int speed,
    int vBatt,
    double vAux,
    double aMotor,
    double aShunt,
    int temp)
{
    String msg =
        String(unixTime) + ";" +
        String(speed) + ";" +
        String(vBatt) + ";" +
        String(vAux) + ";" +
        String(aMotor) + ";" +
        String(aShunt) + ";" +
        String(temp);

    LoraSerial::queueData(msg);
}

bool LoraSerial::hasSent()
{
    return millis() < (_timeSent + 500);
}

String LoraSerial::parseData()
{
    String msg = LoraSerial::readString();
    String parsed[5];
    int n = 0;
    int length = msg.length();
    String tmp;

    // split raw data using commas
    for (int i = 0; i < length; i++)
    {
        char c = msg[i];
        if (c == ',')
        {
            parsed[n] = tmp;
            tmp = "";
            n++;
        }
        else
        {
            tmp += String(c);
        }
    }
    // return 3rd separated element (the message)
    return parsed[2];
}