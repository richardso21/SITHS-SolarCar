#include "LoraSerial.hpp"

void LoraSerial::sendData(String message)
{
    int size = message.length();
    String res = "AT+SEND=0," + String(size) + "," + message + "\r\n";
    LoraSerial::print(res);
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